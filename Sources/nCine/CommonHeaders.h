#include <CommonBase.h>

#if defined(NCINE_INCLUDE_OPENAL)
#	if !defined(AL_ALEXT_PROTOTYPES)
#		define AL_ALEXT_PROTOTYPES
#	endif
#	if defined(DEATH_TARGET_APPLE)
#		include <OpenAL/al.h>
#	elif defined(DEATH_TARGET_EMSCRIPTEN)
#		include <AL/al.h>
#	else
#		if !defined(CMAKE_BUILD) && defined(__has_include)
#			if __has_include("AL/al.h")
#				define __HAS_LOCAL_OPENAL
#			endif
#		endif
#		if defined(__HAS_LOCAL_OPENAL)
#			include "AL/al.h"
#			include "AL/alext.h"
#		else
#			include <al.h>
#			include <alext.h>
#		endif
#		define OPENAL_FILTERS_SUPPORTED
#	endif
#endif

#if defined(NCINE_INCLUDE_OPENALC)
#	if !defined(AL_ALEXT_PROTOTYPES)
#		define AL_ALEXT_PROTOTYPES
#	endif
#	if defined(DEATH_TARGET_APPLE)
#		include <OpenAL/alc.h>
#		include <OpenAL/al.h>
#	elif defined(DEATH_TARGET_EMSCRIPTEN)
#		include <AL/alc.h>
#		include <AL/al.h>
#	else
#		if !defined(CMAKE_BUILD) && defined(__has_include)
#			if __has_include("AL/al.h")
#				define __HAS_LOCAL_OPENALC
#			endif
#		endif
#		if defined(__HAS_LOCAL_OPENALC)
#			include "AL/alc.h"
#			include "AL/al.h"
#			include "AL/alext.h"
#			if __has_include("AL/inprogext.h")
#				include "AL/inprogext.h"
#			endif
#		else
#			include <alc.h>
#			include <al.h>
#			include <alext.h>
#			if defined(__has_include)
#				if __has_include(<inprogext.h>)
#					include <inprogext.h>
#				endif
#			endif
#		endif
#	endif
#endif