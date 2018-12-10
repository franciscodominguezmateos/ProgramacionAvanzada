################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cilindro.cpp \
../src/cubo.cpp \
../src/escena.cpp \
../src/esfera.cpp \
../src/motorJuegosOpenGLbaseOOclases.cpp \
../src/rosco.cpp \
../src/solido.cpp \
../src/vector3d.cpp 

OBJS += \
./src/cilindro.o \
./src/cubo.o \
./src/escena.o \
./src/esfera.o \
./src/motorJuegosOpenGLbaseOOclases.o \
./src/rosco.o \
./src/solido.o \
./src/vector3d.o 

CPP_DEPS += \
./src/cilindro.d \
./src/cubo.d \
./src/escena.d \
./src/esfera.d \
./src/motorJuegosOpenGLbaseOOclases.d \
./src/rosco.d \
./src/solido.d \
./src/vector3d.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


