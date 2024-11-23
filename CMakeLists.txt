cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

# Policies
if(POLICY CMP0127)
	cmake_policy(SET CMP0127 NEW)
endif()
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
endif()
if(POLICY CMP0144)
	cmake_policy(SET CMP0144 NEW)
endif()

set(NCINE_ROOT ${CMAKE_SOURCE_DIR})
set(NCINE_SOURCE_DIR "${NCINE_ROOT}/Sources")
set(NCINE_APP "jazz2")
set(NCINE_APP_NAME "Jazz² Resurrection")
set(NCINE_APP_DESCRIPTION "Open-source reimplementation of Jazz Jackrabbit 2")
set(NCINE_APP_DESCRIPTION_FULL "Jazz² Resurrection is reimplementation of the game Jazz Jackrabbit 2 released in 1998. Supports various versions of the game (Shareware Demo, Holiday Hare '98, The Secret Files and Christmas Chronicles). Also, it partially supports some features of JJ2+ extension and MLLE.\n\nFurther information can be found here: https://deat.tk/jazz2/")
set(NCINE_APP_VENDOR "Dan R.")
set(NCINE_REVERSE_DNS "jazz2.resurrection")
set(NCINE_VERSION "2.9.1")
set(NCINE_PREFERRED_BACKEND "SDL2")
message(STATUS "VITA")
	

if( DEFINED ENV{VITASDK} )
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
else()
	message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
endif()
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

if(NOT IS_DIRECTORY ${NCINE_DATA_DIR})
	message(WARNING "Content directory not found at: ${NCINE_DATA_DIR}")
else()
	message(STATUS "Content directory: ${NCINE_DATA_DIR}")
endif()



# vita stuff

set(VITA_APPNAME "Jazz2")
set(VITA_TITLEID "RCATJAZZ2")
set(VITA_VERSION "01.00")


project(${VITA_APPNAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDATAPATH=\\\"ux0:data/jazz2/\\\"")



include_directories(
	${VITASDK}/arm-vita-eabi/include/SDL
	${VITASDK}/arm-vita-eabi/include/SDL2
	${CMAKE_SOURCE_DIR}/Sources
	${CMAKE_SOURCE_DIR}/Sources/Shared
	${CMAKE_SOURCE_DIR}/Sources/Jazz2
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Collectibles
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Enemies
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Enemies/Bosses
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Environment
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Lighting
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Multiplayer
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Solid
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Actors/Weapons	
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Collisions
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Compatibility
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Events
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Multiplayer
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Multiplayer/Backends
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Scripting
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/Tiles
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/UI
	${CMAKE_SOURCE_DIR}/Sources/Jazz2/UI/Menu
	${CMAKE_SOURCE_DIR}/Sources/nCine
	${CMAKE_SOURCE_DIR}/Sources/nCine/Audio
	${CMAKE_SOURCE_DIR}/Sources/nCine/Backends
	${CMAKE_SOURCE_DIR}/Sources/nCine/Backends/Android
	${CMAKE_SOURCE_DIR}/Sources/nCine/Backends/Uwp
	${CMAKE_SOURCE_DIR}/Sources/nCine/Base
	${CMAKE_SOURCE_DIR}/Sources/nCine/Base/ParallelHashMap
	${CMAKE_SOURCE_DIR}/Sources/nCine/Base/pdqsort
	${CMAKE_SOURCE_DIR}/Sources/nCine/Graphics

	${CMAKE_SOURCE_DIR}/Sources/nCine/Input
	${CMAKE_SOURCE_DIR}/Sources/nCine/IO
	${CMAKE_SOURCE_DIR}/Sources/nCine/Primitives
	${CMAKE_SOURCE_DIR}/Sources/nCine/Shaders
	${CMAKE_SOURCE_DIR}/Sources/nCine/Threading
	${CMAKE_SOURCE_DIR}/Sources/Shared
	${CMAKE_SOURCE_DIR}/Sources/Shared/Base
	${CMAKE_SOURCE_DIR}/Sources/Shared/Containers
	${CMAKE_SOURCE_DIR}/Sources/Shared/Core
	${CMAKE_SOURCE_DIR}/Sources/Shared/IO
	${CMAKE_SOURCE_DIR}/Sources/Shared/Threading
	${CMAKE_SOURCE_DIR}/Sources/Shared/Threading/Implementation
	${CMAKE_SOURCE_DIR}/Sources/simdjson
)


link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)


# Organize main project files into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PATH_PREFIX ${NCINE_SOURCE_DIR})
foreach(FILE ${HEADERS} ${SOURCES}) 
	get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
	string(LENGTH "${PATH_PREFIX}" PATH_PREFIX_LENGTH)
	string(SUBSTRING "${PARENT_DIR}" 0 ${PATH_PREFIX_LENGTH} PATH_START)
	if(PATH_START STREQUAL "${PATH_PREFIX}")
		string(SUBSTRING "${PARENT_DIR}" ${PATH_PREFIX_LENGTH} -1 GROUP)
		string(REPLACE "/" "\\" GROUP "${GROUP}")

		# Group into "Source Files" and "Header Files"
		if("${FILE}" MATCHES ".*\\.cpp")
			set(GROUP "Source Files${GROUP}")
		elseif("${FILE}" MATCHES ".*\\.h")
			set(GROUP "Header Files${GROUP}")
		endif()
	else()
		# Path doesn't have common prefix, put it into "Dependencies"
		set(GROUP "Dependencies")
	endif()

	source_group("${GROUP}" FILES "${FILE}")
endforeach()

foreach(SOURCE_FILE IN LISTS SHADER_FILES)
	source_group("Shaders" FILES ${SOURCE_FILE})
endforeach()
foreach(SOURCE_FILE ${GENERATED_SOURCES})
	source_group("Generated Files" FILES ${SOURCE_FILE})
endforeach()

#more vita
add_executable(${VITA_APPNAME}
Sources/nCine/AppConfiguration.cpp
Sources/nCine/Application.cpp
Sources/nCine/Audio/ALAudioDevice.cpp
Sources/nCine/Audio/AudioBuffer.cpp
Sources/nCine/Audio/AudioBufferPlayer.cpp
Sources/nCine/Audio/AudioLoaderMpt.cpp
Sources/nCine/Audio/AudioLoaderOgg.cpp
Sources/nCine/Audio/AudioLoaderWav.cpp
Sources/nCine/Audio/AudioReaderMpt.cpp
Sources/nCine/Audio/AudioReaderOgg.cpp
Sources/nCine/Audio/AudioReaderWav.cpp
Sources/nCine/Audio/AudioStream.cpp
Sources/nCine/Audio/AudioStreamPlayer.cpp
Sources/nCine/Audio/IAudioLoader.cpp
Sources/nCine/Audio/IAudioPlayer.cpp
Sources/nCine/Backends/Android/AndroidApplication.cpp
Sources/nCine/Backends/Android/AndroidInputManager.cpp
Sources/nCine/Backends/Android/AndroidJniHelper.cpp
Sources/nCine/Backends/Android/AndroidKeys.cpp
Sources/nCine/Backends/Android/EglGfxDevice.cpp
Sources/nCine/Backends/Android/ImGuiAndroidInput.cpp
Sources/nCine/Backends/GlfwGfxDevice.cpp
Sources/nCine/Backends/GlfwInputManager.cpp
Sources/nCine/Backends/GlfwKeys.cpp
Sources/nCine/Backends/ImGuiGlfwInput.cpp
Sources/nCine/Backends/ImGuiSdlInput.cpp
Sources/nCine/Backends/Qt5GfxDevice.cpp
Sources/nCine/Backends/Qt5InputManager.cpp
Sources/nCine/Backends/Qt5JoyMapping.cpp
Sources/nCine/Backends/Qt5Keys.cpp
Sources/nCine/Backends/Qt5Widget.cpp
Sources/nCine/Backends/SdlGfxDevice.cpp
Sources/nCine/Backends/SdlInputManager.cpp
Sources/nCine/Backends/SdlKeys.cpp
Sources/nCine/Backends/Uwp/UwpApplication.cpp
Sources/nCine/Backends/Uwp/UwpGfxDevice.cpp
Sources/nCine/Backends/Uwp/UwpInputManager.cpp
Sources/nCine/Base/Algorithms.cpp
Sources/nCine/Base/BitArray.cpp
Sources/nCine/Base/Clock.cpp
Sources/nCine/Base/FrameTimer.cpp
Sources/nCine/Base/HashFunctions.cpp
Sources/nCine/Base/Object.cpp
Sources/nCine/Base/Random.cpp
Sources/nCine/Base/Timer.cpp
Sources/nCine/Base/TimeStamp.cpp
Sources/nCine/Graphics/AnimatedSprite.cpp
Sources/nCine/Graphics/BaseSprite.cpp
Sources/nCine/Graphics/BinaryShaderCache.cpp
Sources/nCine/Graphics/Camera.cpp
Sources/nCine/Graphics/DrawableNode.cpp
Sources/nCine/Graphics/Geometry.cpp
Sources/nCine/Graphics/GfxCapabilities.cpp
Sources/nCine/Graphics/GL/GLAttribute.cpp
Sources/nCine/Graphics/GL/GLBlending.cpp
Sources/nCine/Graphics/GL/GLBufferObject.cpp
Sources/nCine/Graphics/GL/GLClearColor.cpp
Sources/nCine/Graphics/GL/GLCullFace.cpp
Sources/nCine/Graphics/GL/GLDebug.cpp
Sources/nCine/Graphics/GL/GLDepthTest.cpp
Sources/nCine/Graphics/GL/GLFramebuffer.cpp
Sources/nCine/Graphics/GL/GLRenderbuffer.cpp
Sources/nCine/Graphics/GL/GLScissorTest.cpp
Sources/nCine/Graphics/GL/GLShader.cpp
Sources/nCine/Graphics/GL/GLShaderProgram.cpp
Sources/nCine/Graphics/GL/GLShaderUniformBlocks.cpp
Sources/nCine/Graphics/GL/GLShaderUniforms.cpp
Sources/nCine/Graphics/GL/GLTexture.cpp
Sources/nCine/Graphics/GL/GLUniform.cpp
Sources/nCine/Graphics/GL/GLUniformBlock.cpp
Sources/nCine/Graphics/GL/GLUniformBlockCache.cpp
Sources/nCine/Graphics/GL/GLUniformCache.cpp
Sources/nCine/Graphics/GL/GLVertexArrayObject.cpp
Sources/nCine/Graphics/GL/GLVertexFormat.cpp
Sources/nCine/Graphics/GL/GLViewport.cpp

Sources/nCine/Graphics/IGfxDevice.cpp
Sources/nCine/Graphics/ImGuiDebugOverlay.cpp
Sources/nCine/Graphics/ImGuiDrawing.cpp
Sources/nCine/Graphics/ITextureLoader.cpp
Sources/nCine/Graphics/ITextureSaver.cpp
Sources/nCine/Graphics/Material.cpp
Sources/nCine/Graphics/MeshSprite.cpp
Sources/nCine/Graphics/Particle.cpp
Sources/nCine/Graphics/ParticleAffectors.cpp
Sources/nCine/Graphics/ParticleInitializer.cpp
Sources/nCine/Graphics/ParticleSystem.cpp
Sources/nCine/Graphics/RectAnimation.cpp
Sources/nCine/Graphics/RenderBatcher.cpp
Sources/nCine/Graphics/RenderBuffersManager.cpp
Sources/nCine/Graphics/RenderCommand.cpp
Sources/nCine/Graphics/RenderCommandPool.cpp
Sources/nCine/Graphics/RenderDocCapture.cpp
Sources/nCine/Graphics/RenderQueue.cpp
Sources/nCine/Graphics/RenderResources.cpp
Sources/nCine/Graphics/RenderStatistics.cpp
Sources/nCine/Graphics/RenderVaoPool.cpp
Sources/nCine/Graphics/SceneNode.cpp
Sources/nCine/Graphics/ScreenViewport.cpp
Sources/nCine/Graphics/Shader.cpp
Sources/nCine/Graphics/ShaderState.cpp
Sources/nCine/Graphics/Sprite.cpp
Sources/nCine/Graphics/Texture.cpp
Sources/nCine/Graphics/TextureFormat.cpp
Sources/nCine/Graphics/TextureLoaderDds.cpp
Sources/nCine/Graphics/TextureLoaderKtx.cpp
Sources/nCine/Graphics/TextureLoaderPkm.cpp
Sources/nCine/Graphics/TextureLoaderPng.cpp
Sources/nCine/Graphics/TextureLoaderPvr.cpp
Sources/nCine/Graphics/TextureLoaderQoi.cpp
Sources/nCine/Graphics/TextureLoaderRaw.cpp
Sources/nCine/Graphics/TextureLoaderWebP.cpp
Sources/nCine/Graphics/TextureSaverPng.cpp
Sources/nCine/Graphics/TextureSaverWebP.cpp
Sources/nCine/Graphics/Viewport.cpp
Sources/nCine/I18n.cpp
Sources/nCine/Input/IInputManager.cpp
Sources/nCine/Input/ImGuiJoyMappedInput.cpp
Sources/nCine/Input/JoyMapping.cpp
Sources/nCine/IO/EmscriptenLocalFile.cpp
Sources/nCine/MainApplication.cpp
Sources/nCine/Primitives/Color.cpp
Sources/nCine/Primitives/Colorf.cpp
Sources/nCine/ServiceLocator.cpp
Sources/nCine/Threading/GccAtomic.cpp
Sources/nCine/Threading/PosixThread.cpp
Sources/nCine/Threading/PosixThreadSync.cpp
Sources/nCine/Threading/StdAtomic.cpp
Sources/nCine/Threading/ThreadPool.cpp
Sources/nCine/Threading/WindowsAtomic.cpp
Sources/nCine/Threading/WindowsThread.cpp
Sources/nCine/Threading/WindowsThreadSync.cpp
Sources/nCine/tracy_memory.cpp
Sources/Shared/Containers/DateTime.cpp
Sources/Shared/Containers/SmallVector.cpp
Sources/Shared/Containers/String.cpp
Sources/Shared/Containers/StringUtils.cpp
Sources/Shared/Containers/StringView.cpp
Sources/Shared/Core/Logger.cpp
Sources/Shared/Cpu.cpp
Sources/Shared/Environment.cpp
Sources/Shared/IO/AndroidAssetStream.cpp
Sources/Shared/IO/DeflateStream.cpp
Sources/Shared/IO/FileStream.cpp
Sources/Shared/IO/FileSystem.cpp
Sources/Shared/IO/MemoryStream.cpp
Sources/Shared/IO/PakFile.cpp
Sources/Shared/IO/Stream.cpp
Sources/Shared/Threading/Implementation/WaitOnAddress.cpp
Sources/Shared/Utf8.cpp
Sources/simdjson/simdjson.cpp


Sources/Jazz2/Actors/ActorBase.cpp
Sources/Jazz2/Actors/Collectibles/AmmoCollectible.cpp
Sources/Jazz2/Actors/Collectibles/CarrotCollectible.cpp
Sources/Jazz2/Actors/Collectibles/CarrotFlyCollectible.cpp
Sources/Jazz2/Actors/Collectibles/CarrotInvincibleCollectible.cpp    
Sources/Jazz2/Actors/Collectibles/CoinCollectible.cpp
Sources/Jazz2/Actors/Collectibles/CollectibleBase.cpp
Sources/Jazz2/Actors/Collectibles/FastFireCollectible.cpp
Sources/Jazz2/Actors/Collectibles/FoodCollectible.cpp
Sources/Jazz2/Actors/Collectibles/GemCollectible.cpp
Sources/Jazz2/Actors/Collectibles/GemGiant.cpp
Sources/Jazz2/Actors/Collectibles/GemRing.cpp
Sources/Jazz2/Actors/Collectibles/OneUpCollectible.cpp
Sources/Jazz2/Actors/Collectibles/Stopwatch.cpp
Sources/Jazz2/Actors/Enemies/Bat.cpp
Sources/Jazz2/Actors/Enemies/Bee.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Bilsy.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Bolly.cpp
Sources/Jazz2/Actors/Enemies/Bosses/BossBase.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Bubba.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Devan.cpp
Sources/Jazz2/Actors/Enemies/Bosses/DevanRemote.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Queen.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Robot.cpp
Sources/Jazz2/Actors/Enemies/Bosses/TurtleBoss.cpp
Sources/Jazz2/Actors/Enemies/Bosses/Uterus.cpp
Sources/Jazz2/Actors/Enemies/Caterpillar.cpp
Sources/Jazz2/Actors/Enemies/Crab.cpp
Sources/Jazz2/Actors/Enemies/Demon.cpp
Sources/Jazz2/Actors/Enemies/Doggy.cpp
Sources/Jazz2/Actors/Enemies/Dragon.cpp
Sources/Jazz2/Actors/Enemies/Dragonfly.cpp
Sources/Jazz2/Actors/Enemies/EnemyBase.cpp
Sources/Jazz2/Actors/Enemies/FatChick.cpp
Sources/Jazz2/Actors/Enemies/Fencer.cpp
Sources/Jazz2/Actors/Enemies/Fish.cpp
Sources/Jazz2/Actors/Enemies/Helmut.cpp
Sources/Jazz2/Actors/Enemies/LabRat.cpp
Sources/Jazz2/Actors/Enemies/Lizard.cpp
Sources/Jazz2/Actors/Enemies/LizardFloat.cpp
Sources/Jazz2/Actors/Enemies/MadderHatter.cpp
Sources/Jazz2/Actors/Enemies/Monkey.cpp
Sources/Jazz2/Actors/Enemies/Rapier.cpp
Sources/Jazz2/Actors/Enemies/Raven.cpp
Sources/Jazz2/Actors/Enemies/Skeleton.cpp
Sources/Jazz2/Actors/Enemies/Sparks.cpp
Sources/Jazz2/Actors/Enemies/Sucker.cpp
Sources/Jazz2/Actors/Enemies/SuckerFloat.cpp
Sources/Jazz2/Actors/Enemies/Turtle.cpp
Sources/Jazz2/Actors/Enemies/TurtleShell.cpp
Sources/Jazz2/Actors/Enemies/TurtleTough.cpp
Sources/Jazz2/Actors/Enemies/TurtleTube.cpp
Sources/Jazz2/Actors/Enemies/Witch.cpp
Sources/Jazz2/Actors/Environment/AirboardGenerator.cpp
Sources/Jazz2/Actors/Environment/AmbientBubbles.cpp
Sources/Jazz2/Actors/Environment/AmbientSound.cpp
Sources/Jazz2/Actors/Environment/Bird.cpp
Sources/Jazz2/Actors/Environment/BirdCage.cpp
Sources/Jazz2/Actors/Environment/Bomb.cpp
Sources/Jazz2/Actors/Environment/BonusWarp.cpp
Sources/Jazz2/Actors/Environment/Checkpoint.cpp
Sources/Jazz2/Actors/Environment/Copter.cpp
Sources/Jazz2/Actors/Environment/EndOfLevel.cpp
Sources/Jazz2/Actors/Environment/Eva.cpp
Sources/Jazz2/Actors/Environment/IceBlock.cpp
Sources/Jazz2/Actors/Environment/Moth.cpp
Sources/Jazz2/Actors/Environment/RollingRock.cpp
Sources/Jazz2/Actors/Environment/Spring.cpp
Sources/Jazz2/Actors/Environment/SteamNote.cpp
Sources/Jazz2/Actors/Environment/SwingingVine.cpp
Sources/Jazz2/Actors/Explosion.cpp
Sources/Jazz2/Actors/Lighting/FlickerLight.cpp
Sources/Jazz2/Actors/Lighting/PulsatingRadialLight.cpp
Sources/Jazz2/Actors/Lighting/StaticRadialLight.cpp
Sources/Jazz2/Actors/Multiplayer/LocalPlayerOnServer.cpp
Sources/Jazz2/Actors/Multiplayer/PlayerOnServer.cpp
Sources/Jazz2/Actors/Multiplayer/RemotablePlayer.cpp
Sources/Jazz2/Actors/Multiplayer/RemoteActor.cpp
Sources/Jazz2/Actors/Multiplayer/RemotePlayerOnServer.cpp
Sources/Jazz2/Actors/Player.cpp
Sources/Jazz2/Actors/PlayerCorpse.cpp
Sources/Jazz2/Actors/Solid/AmmoBarrel.cpp
Sources/Jazz2/Actors/Solid/AmmoCrate.cpp
Sources/Jazz2/Actors/Solid/BarrelContainer.cpp
Sources/Jazz2/Actors/Solid/Bridge.cpp
Sources/Jazz2/Actors/Solid/CrateContainer.cpp
Sources/Jazz2/Actors/Solid/GemBarrel.cpp
Sources/Jazz2/Actors/Solid/GemCrate.cpp
Sources/Jazz2/Actors/Solid/GenericContainer.cpp
Sources/Jazz2/Actors/Solid/MovingPlatform.cpp
Sources/Jazz2/Actors/Solid/PinballBumper.cpp
Sources/Jazz2/Actors/Solid/PinballPaddle.cpp
Sources/Jazz2/Actors/Solid/Pole.cpp
Sources/Jazz2/Actors/Solid/PowerUpMorphMonitor.cpp
Sources/Jazz2/Actors/Solid/PowerUpShieldMonitor.cpp
Sources/Jazz2/Actors/Solid/PowerUpWeaponMonitor.cpp
Sources/Jazz2/Actors/Solid/PushableBox.cpp
Sources/Jazz2/Actors/Solid/SpikeBall.cpp
Sources/Jazz2/Actors/Solid/TriggerCrate.cpp
Sources/Jazz2/Actors/SolidObjectBase.cpp
Sources/Jazz2/Actors/Weapons/BlasterShot.cpp
Sources/Jazz2/Actors/Weapons/BouncerShot.cpp
Sources/Jazz2/Actors/Weapons/ElectroShot.cpp
Sources/Jazz2/Actors/Weapons/FreezerShot.cpp
Sources/Jazz2/Actors/Weapons/PepperShot.cpp
Sources/Jazz2/Actors/Weapons/RFShot.cpp
Sources/Jazz2/Actors/Weapons/SeekerShot.cpp
Sources/Jazz2/Actors/Weapons/ShieldFireShot.cpp
Sources/Jazz2/Actors/Weapons/ShieldLightningShot.cpp
Sources/Jazz2/Actors/Weapons/ShieldWaterShot.cpp
Sources/Jazz2/Actors/Weapons/ShotBase.cpp
Sources/Jazz2/Actors/Weapons/Thunderbolt.cpp
Sources/Jazz2/Actors/Weapons/TNT.cpp
Sources/Jazz2/Actors/Weapons/ToasterShot.cpp
Sources/Jazz2/Collisions/DynamicTree.cpp
Sources/Jazz2/Collisions/DynamicTreeBroadPhase.cpp
Sources/Jazz2/Compatibility/AnimSetMapping.cpp
Sources/Jazz2/Compatibility/EventConverter.cpp
Sources/Jazz2/Compatibility/JJ2Anims.cpp
Sources/Jazz2/Compatibility/JJ2Block.cpp
Sources/Jazz2/Compatibility/JJ2Data.cpp
Sources/Jazz2/Compatibility/JJ2Episode.cpp
Sources/Jazz2/Compatibility/JJ2Level.cpp
Sources/Jazz2/Compatibility/JJ2Strings.cpp
Sources/Jazz2/Compatibility/JJ2Tileset.cpp
Sources/Jazz2/ContentResolver.cpp
Sources/Jazz2/Events/EventMap.cpp
Sources/Jazz2/Events/EventSpawner.cpp
Sources/Jazz2/LevelHandler.cpp
Sources/Jazz2/Multiplayer/ConnectionResult.cpp
Sources/Jazz2/Multiplayer/MultiLevelHandler.cpp
Sources/Jazz2/Multiplayer/NetworkManager.cpp
Sources/Jazz2/Multiplayer/ServerDiscovery.cpp
Sources/Jazz2/PlayerViewport.cpp
Sources/Jazz2/PreferencesCache.cpp
Sources/Jazz2/Resources.cpp
Sources/Jazz2/RumbleProcessor.cpp
Sources/Jazz2/Scripting/JJ2PlusDefinitions.cpp
Sources/Jazz2/Scripting/LevelScriptLoader.cpp
Sources/Jazz2/Scripting/RegisterArray.cpp
Sources/Jazz2/Scripting/RegisterDictionary.cpp
Sources/Jazz2/Scripting/RegisterImGuiBindings.cpp
Sources/Jazz2/Scripting/RegisterMath.cpp
Sources/Jazz2/Scripting/RegisterRef.cpp
Sources/Jazz2/Scripting/RegisterString.cpp
Sources/Jazz2/Scripting/ScriptActorWrapper.cpp
Sources/Jazz2/Scripting/ScriptLoader.cpp
Sources/Jazz2/Scripting/ScriptPlayerWrapper.cpp
Sources/Jazz2/Tiles/TileMap.cpp
Sources/Jazz2/Tiles/TileSet.cpp
Sources/Jazz2/UI/Canvas.cpp
Sources/Jazz2/UI/Cinematics.cpp
Sources/Jazz2/UI/ControlScheme.cpp
Sources/Jazz2/UI/DiscordRpcClient.cpp
Sources/Jazz2/UI/Font.cpp
Sources/Jazz2/UI/FormattedTextBlock.cpp
Sources/Jazz2/UI/HUD.cpp
Sources/Jazz2/UI/LoadingHandler.cpp
Sources/Jazz2/UI/Menu/AboutSection.cpp
Sources/Jazz2/UI/Menu/BeginSection.cpp
Sources/Jazz2/UI/Menu/ControlsOptionsSection.cpp
Sources/Jazz2/UI/Menu/CreateServerOptionsSection.cpp
Sources/Jazz2/UI/Menu/CustomLevelSelectSection.cpp
Sources/Jazz2/UI/Menu/EpisodeSelectSection.cpp
Sources/Jazz2/UI/Menu/FirstRunSection.cpp
Sources/Jazz2/UI/Menu/GameplayEnhancementsSection.cpp
Sources/Jazz2/UI/Menu/GameplayOptionsSection.cpp
Sources/Jazz2/UI/Menu/GraphicsOptionsSection.cpp
Sources/Jazz2/UI/Menu/ImportSection.cpp
Sources/Jazz2/UI/Menu/InGameMenu.cpp
Sources/Jazz2/UI/Menu/InputDiagnosticsSection.cpp
Sources/Jazz2/UI/Menu/LanguageSelectSection.cpp
Sources/Jazz2/UI/Menu/LoadingSection.cpp
Sources/Jazz2/UI/Menu/MainMenu.cpp
Sources/Jazz2/UI/Menu/MultiplayerGameModeSelectSection.cpp
Sources/Jazz2/UI/Menu/OptionsSection.cpp
Sources/Jazz2/UI/Menu/PauseSection.cpp
Sources/Jazz2/UI/Menu/PlayCustomSection.cpp
Sources/Jazz2/UI/Menu/RefreshCacheSection.cpp
Sources/Jazz2/UI/Menu/RemapControlsSection.cpp
Sources/Jazz2/UI/Menu/RescaleModeSection.cpp
Sources/Jazz2/UI/Menu/ServerSelectSection.cpp
Sources/Jazz2/UI/Menu/SimpleMessageSection.cpp
Sources/Jazz2/UI/Menu/SoundsOptionsSection.cpp
Sources/Jazz2/UI/Menu/StartGameOptionsSection.cpp
Sources/Jazz2/UI/Menu/TouchControlsOptionsSection.cpp
Sources/Jazz2/UI/RgbLights.cpp
Sources/Jazz2/UI/UpscaleRenderPass.cpp

Sources/Main.cpp
)
target_link_libraries(${VITA_APPNAME}
SDL2
imgui
vitaGL
vitashark
mathneon
modplug
kbdvita
z
SceAppMgr_stub
SceAppUtil_stub
SceAudio_stub
SceCommonDialog_stub
SceCtrl_stub
SceDisplay_stub
SceGxm_stub
SceHid_stub
ScePower_stub
SceShaccCg_stub
SceSysmodule_stub
SceTouch_stub
)

vita_create_self(eboot.bin ${VITA_APPNAME} UNSAFE)
vita_create_vpk(${VITA_APPNAME}.vpk ${VITA_TITLEID} eboot.bin
VERSION ${VITA_VERSION}
NAME ${VITA_APPNAME}
#FILE res/icon0.png sce_sys/icon0.png
#FILE res/bg.png sce_sys/livearea/contents/bg.png
#FILE res/startup.png sce_sys/livearea/contents/startup.png
#FILE res/template.xml sce_sys/livearea/contents/template.xml
)

#cmake -D CMAKE_TOOLCHAIN_FILE=${VITASDK}/share/vita.toolchain.cmake -D NCINE_PREFERRED_BACKEND=SDL2 ..