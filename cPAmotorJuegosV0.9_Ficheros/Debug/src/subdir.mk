################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cilindro.cpp \
../src/compuesto.cpp \
../src/cubo.cpp \
../src/escena.cpp \
../src/esfera.cpp \
../src/modelo.cpp \
../src/motorJuegosOpenGLbaseOOclases.cpp \
../src/plano.cpp \
../src/rosco.cpp \
../src/solido.cpp \
../src/triangulo.cpp \
../src/vector3d.cpp 

OBJS += \
./src/cilindro.o \
./src/compuesto.o \
./src/cubo.o \
./src/escena.o \
./src/esfera.o \
./src/modelo.o \
./src/motorJuegosOpenGLbaseOOclases.o \
./src/plano.o \
./src/rosco.o \
./src/solido.o \
./src/triangulo.o \
./src/vector3d.o 

CPP_DEPS += \
./src/cilindro.d \
./src/compuesto.d \
./src/cubo.d \
./src/escena.d \
./src/esfera.d \
./src/modelo.d \
./src/motorJuegosOpenGLbaseOOclases.d \
./src/plano.d \
./src/rosco.d \
./src/solido.d \
./src/triangulo.d \
./src/vector3d.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


