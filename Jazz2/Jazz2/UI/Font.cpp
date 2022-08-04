﻿#include "Font.h"

#include "../ContentResolver.h"

#include "../../nCine/Graphics/ITextureLoader.h"
#include "../../nCine/Graphics/RenderQueue.h"
#include "../../nCine/IO/IFileStream.h"
#include "../../nCine/Base/Random.h"

#include <Utf8.h>

namespace Jazz2::UI
{
	Font::Font(const StringView& path)
		:
		_baseSpacing(0),
		_charHeight(0)
	{
		auto fileHandle = IFileStream::createFileHandle(path + ".font");
		fileHandle->Open(FileAccessMode::Read);
		auto fileSize = fileHandle->GetSize();
		if (fileSize < 4 || fileSize > 8 * 1024 * 1024) {
			// 8 MB file size limit
			return;
		}

		std::unique_ptr<ITextureLoader> texLoader = ITextureLoader::createFromFile(path);
		if (texLoader->hasLoaded()) {
			auto texFormat = texLoader->texFormat().internalFormat();
			if (texFormat != GL_RGBA8 && texFormat != GL_RGB8) {
				return;
			}

			int w = texLoader->width();
			int h = texLoader->height();
			auto pixels = (uint32_t*)texLoader->pixels();
			const uint32_t* palette = ContentResolver::Current().GetPalettes();

			uint8_t flags = fileHandle->ReadValue<uint8_t>();
			uint16_t width = fileHandle->ReadValue<uint16_t>();
			uint16_t height = fileHandle->ReadValue<uint16_t>();
			uint8_t cols = fileHandle->ReadValue<uint8_t>();
			int rows = h / height;
			int16_t spacing = fileHandle->ReadValue<int16_t>();
			uint8_t asciiFirst = fileHandle->ReadValue<uint8_t>();
			uint8_t asciiCount = fileHandle->ReadValue<uint8_t>();

			uint8_t widths[128];
			fileHandle->Read(widths, asciiCount);

			int i = 0;
			for (; i < asciiCount; i++) {
				_asciiChars[i + asciiFirst] = Rectf(
					(float)(i % cols) / cols,
					(float)(i / cols) / rows,
					widths[i],
					height
				);
			}

			int unicodeCharCount = asciiCount + fileHandle->ReadValue<int32_t>();
			for (; i < unicodeCharCount; i++) {
				char c[4] = { };
				fileHandle->Read(c, 1);

				int remainingBytes =
					((c[0] & 240) == 240) ? 3 : (
					((c[0] & 224) == 224) ? 2 : (
					((c[0] & 192) == 192) ? 1 : -1
				));
				if (remainingBytes == -1) {
					return;
				}

				fileHandle->Read(c + 1, remainingBytes);
				uint8_t charWidth = fileHandle->ReadValue<uint8_t>();

				std::pair<char32_t, std::size_t> cursor = Death::Utf8::NextChar(c, 0);
				_unicodeChars[cursor.first] = Rectf(
					(float)(i % cols) / cols,
					(float)(i / cols) / rows,
					charWidth,
					height
				);
			}

			_charHeight = height;
			_baseSpacing = spacing;

			for (int i = 0; i < w * h; i++) {
				uint32_t color = palette[pixels[i] & 0xff];
				pixels[i] = (color & 0xffffff) | ((((color >> 24) & 0xff) * ((pixels[i] >> 24) & 0xff) / 255) << 24);
			}

			_texture = std::make_unique<Texture>(path.data(), Texture::Format::RGBA8, w, h);
			_texture->loadFromTexels((unsigned char*)pixels, 0, 0, w, h);
			_texture->setMinFiltering(SamplerFilter::Linear);
			_texture->setMagFiltering(SamplerFilter::Linear);
		}
	}

	void Font::DrawString(Canvas* canvas, const StringView& text, int& charOffset, float x, float y, uint16_t z, Alignment align, Colorf color, float scale, float angleOffset, float varianceX, float varianceY, float speed, float charSpacing, float lineSpacing)
	{
		size_t textSize = text.size();
		if (textSize == 0 || _charHeight <= 0) {
			return;
		}

		// TODO: Revise this
		float phase = canvas->AnimTime * speed * 16.0f;

		// Pre-compute text size
		float totalWidth = 0.0f, lastWidth = 0.0f, totalHeight = 0.0f;
		float charSpacingPre = charSpacing;
		float scalePre = scale;

		int idx = 0;
		do {
			std::pair<char32_t, std::size_t> cursor = Death::Utf8::NextChar(text, idx);

			if (cursor.first == '\n') {
				// New line
				if (lastWidth < totalWidth) {
					lastWidth = totalWidth;
				}
				totalWidth = 0.0f;
				totalHeight += (_charHeight * scale * lineSpacing);
			} else {
				Rectf uvRect;
				if (cursor.first < 128) {
					uvRect = _asciiChars[cursor.first];
				} else {
					auto it = _unicodeChars.find(cursor.first);
					if (it != _unicodeChars.end()) {
						uvRect = it->second;
					} else {
						uvRect = Rectf();
					}
				}

				if (uvRect.W > 0 && uvRect.H > 0) {
					totalWidth += (uvRect.W + _baseSpacing) * charSpacingPre * scalePre;
				}
			}

			idx = cursor.second;
		} while (idx < textSize);

		if (lastWidth < totalWidth) {
			lastWidth = totalWidth;
		}
		totalHeight += (_charHeight * scale * lineSpacing);

		// Rendering
		Vector2f originPos = Vector2f(x - canvas->ViewSize.X * 0.5f, canvas->ViewSize.Y * 0.5f - y);
		switch (align & Alignment::HorizontalMask) {
			case Alignment::Center: originPos.X -= totalWidth * 0.5f; break;
			case Alignment::Right: originPos.X -= totalWidth; break;
		}
		switch (align & Alignment::VerticalMask) {
			case Alignment::Center: originPos.Y += totalHeight * 0.5f; break;
			case Alignment::Bottom: originPos.Y += totalHeight; break;
		}

		float lineStart = originPos.X;

		Vector2i texSize = _texture->size();

		uint16_t zz = z;
		idx = 0;
		do {
			std::pair<char32_t, std::size_t> cursor = Death::Utf8::NextChar(text, idx);

			if (cursor.first == '\n') {
				// New line
				originPos.X = lineStart;
				originPos.Y -= (_charHeight * scale * lineSpacing);
			} else {
				Rectf uvRect;
				if (cursor.first < 128) {
					uvRect = _asciiChars[cursor.first];
				} else {
					auto it = _unicodeChars.find(cursor.first);
					if (it != _unicodeChars.end()) {
						uvRect = it->second;
					} else {
						uvRect = Rectf();
					}
				}

				if (uvRect.W > 0 && uvRect.H > 0) {
					Vector2f pos = Vector2f(originPos);

					if (angleOffset > 0.0f) {
						float currentPhase = (phase + charOffset) * angleOffset * fPi;
						if (speed > 0.0f && charOffset % 2 == 1) {
							currentPhase = -currentPhase;
						}

						pos.X += std::cosf(currentPhase) * varianceX * scale;
						pos.Y += std::sinf(currentPhase) * varianceY * scale;
					}

					pos.X = std::round(pos.X + uvRect.W * scale * 0.5f);
					pos.Y = std::round(pos.Y - uvRect.H * scale * 0.5f);

					Vector4f texCoords = Vector4f(
						uvRect.W / float(texSize.X),
						uvRect.X,
						uvRect.H / float(texSize.Y),
						uvRect.Y
					);

					texCoords.W += texCoords.Z;
					texCoords.Z *= -1;

					auto command = canvas->RentRenderCommand();

					auto instanceBlock = command->material().uniformBlock(Material::InstanceBlockName);
					instanceBlock->uniform(Material::TexRectUniformName)->setFloatVector(texCoords.Data());
					instanceBlock->uniform(Material::SpriteSizeUniformName)->setFloatValue(uvRect.W * scale, uvRect.H * scale);
					instanceBlock->uniform(Material::ColorUniformName)->setFloatVector(color.Data());

					command->setTransformation(Matrix4x4f::Translation(pos.X, pos.Y, 0.0f));
					command->setLayer(zz++);
					command->material().setTexture(*_texture.get());

					canvas->_currentRenderQueue->addCommand(command);

					originPos.X += ((uvRect.W + _baseSpacing) * scale * charSpacing);
				}
			}

			idx = cursor.second;
			charOffset++;
		} while (idx < textSize);
	}
}