################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Assignment.cpp \
../src/Bicycle.cpp \
../src/Bound.cpp \
../src/Constant.cpp \
../src/Edge.cpp \
../src/Guard.cpp \
../src/Inequality.cpp \
../src/Invariant.cpp \
../src/LinearHybridAutomaton.cpp \
../src/LinearPredicate.cpp \
../src/LinearPredicateConjunct.cpp \
../src/LinearTerm.cpp \
../src/Location.cpp \
../src/Variable.cpp 

OBJS += \
./src/Assignment.o \
./src/Bicycle.o \
./src/Bound.o \
./src/Constant.o \
./src/Edge.o \
./src/Guard.o \
./src/Inequality.o \
./src/Invariant.o \
./src/LinearHybridAutomaton.o \
./src/LinearPredicate.o \
./src/LinearPredicateConjunct.o \
./src/LinearTerm.o \
./src/Location.o \
./src/Variable.o 

CPP_DEPS += \
./src/Assignment.d \
./src/Bicycle.d \
./src/Bound.d \
./src/Constant.d \
./src/Edge.d \
./src/Guard.d \
./src/Inequality.d \
./src/Invariant.d \
./src/LinearHybridAutomaton.d \
./src/LinearPredicate.d \
./src/LinearPredicateConjunct.d \
./src/LinearTerm.d \
./src/Location.d \
./src/Variable.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/home/johannes/workspace/Bicycle_New/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

