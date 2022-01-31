cmake_minimum_required(VERSION 3.21)
project(Booking)

set(CMAKE_CXX_STANDARD 20)

add_executable(Booking test.cpp old_booking_providers.h old_trip_manager.h new_booking_providers.h new_trip_manager.h booking.h)
