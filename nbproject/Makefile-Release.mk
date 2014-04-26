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
	${OBJECTDIR}/src/button.o \
	${OBJECTDIR}/src/engine/entity.o \
	${OBJECTDIR}/src/engine/timer.o \
	${OBJECTDIR}/src/graphics/buffered.o \
	${OBJECTDIR}/src/graphics/colour.o \
	${OBJECTDIR}/src/graphics/font.o \
	${OBJECTDIR}/src/graphics/graphics.o \
	${OBJECTDIR}/src/graphics/shaders.o \
	${OBJECTDIR}/src/graphics/text.o \
	${OBJECTDIR}/src/graphics/texture.o \
	${OBJECTDIR}/src/list.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/map/map.o \
	${OBJECTDIR}/src/map/shadow.o \
	${OBJECTDIR}/src/math/math.o \
	${OBJECTDIR}/src/math/vector.o \
	${OBJECTDIR}/src/misc.o \
	${OBJECTDIR}/src/screen.o \
	${OBJECTDIR}/src/screens/game.o \
	${OBJECTDIR}/src/screens/menu.o


# C Compiler Flags
CFLAGS=-lpthread -lSDL2 -lSDL2_ttf -lSDL2_gfx -lm -lGL

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

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
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/shadow ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/button.o: src/button.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/button.o src/button.c

${OBJECTDIR}/src/engine/entity.o: src/engine/entity.c 
	${MKDIR} -p ${OBJECTDIR}/src/engine
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine/entity.o src/engine/entity.c

${OBJECTDIR}/src/engine/timer.o: src/engine/timer.c 
	${MKDIR} -p ${OBJECTDIR}/src/engine
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine/timer.o src/engine/timer.c

${OBJECTDIR}/src/graphics/buffered.o: src/graphics/buffered.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/buffered.o src/graphics/buffered.c

${OBJECTDIR}/src/graphics/colour.o: src/graphics/colour.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/colour.o src/graphics/colour.c

${OBJECTDIR}/src/graphics/font.o: src/graphics/font.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/font.o src/graphics/font.c

${OBJECTDIR}/src/graphics/graphics.o: src/graphics/graphics.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/graphics.o src/graphics/graphics.c

${OBJECTDIR}/src/graphics/shaders.o: src/graphics/shaders.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/shaders.o src/graphics/shaders.c

${OBJECTDIR}/src/graphics/text.o: src/graphics/text.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/text.o src/graphics/text.c

${OBJECTDIR}/src/graphics/texture.o: src/graphics/texture.c 
	${MKDIR} -p ${OBJECTDIR}/src/graphics
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/graphics/texture.o src/graphics/texture.c

${OBJECTDIR}/src/list.o: src/list.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/list.o src/list.c

${OBJECTDIR}/src/main.o: src/main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.c

${OBJECTDIR}/src/map/map.o: src/map/map.c 
	${MKDIR} -p ${OBJECTDIR}/src/map
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/map/map.o src/map/map.c

${OBJECTDIR}/src/map/shadow.o: src/map/shadow.c 
	${MKDIR} -p ${OBJECTDIR}/src/map
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/map/shadow.o src/map/shadow.c

${OBJECTDIR}/src/math/math.o: src/math/math.c 
	${MKDIR} -p ${OBJECTDIR}/src/math
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/math/math.o src/math/math.c

${OBJECTDIR}/src/math/vector.o: src/math/vector.c 
	${MKDIR} -p ${OBJECTDIR}/src/math
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/math/vector.o src/math/vector.c

${OBJECTDIR}/src/misc.o: src/misc.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/misc.o src/misc.c

${OBJECTDIR}/src/screen.o: src/screen.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/screen.o src/screen.c

${OBJECTDIR}/src/screens/game.o: src/screens/game.c 
	${MKDIR} -p ${OBJECTDIR}/src/screens
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/screens/game.o src/screens/game.c

${OBJECTDIR}/src/screens/menu.o: src/screens/menu.c 
	${MKDIR} -p ${OBJECTDIR}/src/screens
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/screens/menu.o src/screens/menu.c

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
