#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Button.o \
	${OBJECTDIR}/src/engine/Entity.o \
	${OBJECTDIR}/src/engine/Timing.o \
	${OBJECTDIR}/src/engine/mobs/Mob.o \
	${OBJECTDIR}/src/graphics/BitmapFont.o \
	${OBJECTDIR}/src/graphics/Buffer.o \
	${OBJECTDIR}/src/graphics/Colours.o \
	${OBJECTDIR}/src/graphics/Graphics.o \
	${OBJECTDIR}/src/graphics/Layout.o \
	${OBJECTDIR}/src/graphics/Shaders.o \
	${OBJECTDIR}/src/graphics/Texture.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/map/Map.o \
	${OBJECTDIR}/src/map/PathFinder.o \
	${OBJECTDIR}/src/misc.o \
	${OBJECTDIR}/src/views/GameView.o \
	${OBJECTDIR}/src/views/MenuView.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=${varCOMPILE}
CXXFLAGS=${varCOMPILE}

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/shadow

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/shadow: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/shadow ${OBJECTFILES} ${LDLIBSOPTIONS} -lGL -lglfw -lGLEW -lm -lpng -lpthread

${OBJECTDIR}/src/Button.o: src/Button.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Button.o src/Button.cc

${OBJECTDIR}/src/engine/Entity.o: src/engine/Entity.cc 
	${MKDIR} -p ${OBJECTDIR}/src/engine
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine/Entity.o src/engine/Entity.cc

${OBJECTDIR}/src/engine/Timing.o: src/engine/Timing.cc 
	${MKDIR} -p ${OBJECTDIR}/src/engine
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine/Timing.o src/engine/Timing.cc

${OBJECTDIR}/src/engine/mobs/Mob.o: src/engine/mobs/Mob.cc 
	${MKDIR} -p ${OBJECTDIR}/src/engine/mobs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine/mobs/Mob.o src/engine/mobs/Mob.cc

${OBJECTDIR}/src/graphics/BitmapFont.o: src/graphics/BitmapFont.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/BitmapFont.o src/graphics/BitmapFont.cc

${OBJECTDIR}/src/graphics/Buffer.o: src/graphics/Buffer.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/Buffer.o src/graphics/Buffer.cc

${OBJECTDIR}/src/graphics/Colours.o: src/graphics/Colours.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/Colours.o src/graphics/Colours.cc

${OBJECTDIR}/src/graphics/Graphics.o: src/graphics/Graphics.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/Graphics.o src/graphics/Graphics.cc

${OBJECTDIR}/src/graphics/Layout.o: src/graphics/Layout.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/Layout.o src/graphics/Layout.cc

${OBJECTDIR}/src/graphics/Shaders.o: src/graphics/Shaders.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/Shaders.o src/graphics/Shaders.cc

${OBJECTDIR}/src/graphics/Texture.o: src/graphics/Texture.cc 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/Texture.o src/graphics/Texture.cc

${OBJECTDIR}/src/main.o: src/main.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cc

${OBJECTDIR}/src/map/Map.o: src/map/Map.cc 
	${MKDIR} -p ${OBJECTDIR}/src/map
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/map/Map.o src/map/Map.cc

${OBJECTDIR}/src/map/PathFinder.o: src/map/PathFinder.cc 
	${MKDIR} -p ${OBJECTDIR}/src/map
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/map/PathFinder.o src/map/PathFinder.cc

${OBJECTDIR}/src/misc.o: src/misc.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/misc.o src/misc.cc

${OBJECTDIR}/src/views/GameView.o: src/views/GameView.cc 
	${MKDIR} -p ${OBJECTDIR}/src/views
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/views/GameView.o src/views/GameView.cc

${OBJECTDIR}/src/views/MenuView.o: src/views/MenuView.cc 
	${MKDIR} -p ${OBJECTDIR}/src/views
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/views/MenuView.o src/views/MenuView.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/shadow

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
