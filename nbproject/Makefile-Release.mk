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
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/map.o \
	${OBJECTDIR}/misc.o \
	${OBJECTDIR}/screen.o \
	${OBJECTDIR}/screens/game.o \
	${OBJECTDIR}/screens/menu.o


# C Compiler Flags
CFLAGS=-lpthread -lSDL2 -lSDL2_ttf -lSDL2_gfx

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdl_game

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdl_game: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdl_game ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -ISDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/map.o: map.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -ISDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/map.o map.c

${OBJECTDIR}/misc.o: misc.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -ISDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/misc.o misc.c

${OBJECTDIR}/screen.o: screen.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -ISDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/screen.o screen.c

${OBJECTDIR}/screens/game.o: screens/game.c 
	${MKDIR} -p ${OBJECTDIR}/screens
	${RM} "$@.d"
	$(COMPILE.c) -O2 -ISDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/screens/game.o screens/game.c

${OBJECTDIR}/screens/menu.o: screens/menu.c 
	${MKDIR} -p ${OBJECTDIR}/screens
	${RM} "$@.d"
	$(COMPILE.c) -O2 -ISDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/screens/menu.o screens/menu.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdl_game

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
