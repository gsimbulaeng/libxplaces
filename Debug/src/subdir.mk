################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/XGetopt.cpp \
../src/xpAction.cpp \
../src/xpAddress.cpp \
../src/xpAliveAction.cpp \
../src/xpAliveEvent.cpp \
../src/xpAnnounceItem.cpp \
../src/xpApplication.cpp \
../src/xpDescriptor.cpp \
../src/xpDescriptorList.cpp \
../src/xpDevice.cpp \
../src/xpDeviceImpl.cpp \
../src/xpDeviceProxy.cpp \
../src/xpDeviceState.cpp \
../src/xpEvent.cpp \
../src/xpEventListener.cpp \
../src/xpEventListenerImpl.cpp \
../src/xpEventListenerProxy.cpp \
../src/xpInitParameters.cpp \
../src/xpMessage.cpp \
../src/xpNetwork.cpp \
../src/xpNode.cpp \
../src/xpProperty.cpp \
../src/xpPropertyList.cpp \
../src/xpToolSet.cpp 

OBJS += \
./src/XGetopt.o \
./src/xpAction.o \
./src/xpAddress.o \
./src/xpAliveAction.o \
./src/xpAliveEvent.o \
./src/xpAnnounceItem.o \
./src/xpApplication.o \
./src/xpDescriptor.o \
./src/xpDescriptorList.o \
./src/xpDevice.o \
./src/xpDeviceImpl.o \
./src/xpDeviceProxy.o \
./src/xpDeviceState.o \
./src/xpEvent.o \
./src/xpEventListener.o \
./src/xpEventListenerImpl.o \
./src/xpEventListenerProxy.o \
./src/xpInitParameters.o \
./src/xpMessage.o \
./src/xpNetwork.o \
./src/xpNode.o \
./src/xpProperty.o \
./src/xpPropertyList.o \
./src/xpToolSet.o 

CPP_DEPS += \
./src/XGetopt.d \
./src/xpAction.d \
./src/xpAddress.d \
./src/xpAliveAction.d \
./src/xpAliveEvent.d \
./src/xpAnnounceItem.d \
./src/xpApplication.d \
./src/xpDescriptor.d \
./src/xpDescriptorList.d \
./src/xpDevice.d \
./src/xpDeviceImpl.d \
./src/xpDeviceProxy.d \
./src/xpDeviceState.d \
./src/xpEvent.d \
./src/xpEventListener.d \
./src/xpEventListenerImpl.d \
./src/xpEventListenerProxy.d \
./src/xpInitParameters.d \
./src/xpMessage.d \
./src/xpNetwork.d \
./src/xpNode.d \
./src/xpProperty.d \
./src/xpPropertyList.d \
./src/xpToolSet.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -fPIC -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


