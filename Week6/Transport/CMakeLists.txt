cmake_minimum_required(VERSION 3.21)
project(Transport)

set(CMAKE_CXX_STANDARD 20)

add_executable(Transport
        main.cpp

        Request/Request.h Request/Request.cpp
        Request/AddBusRequest.h Request/AddBusRequest.cpp
        Request/AddStopRequest.h Request/AddStopRequest.cpp
        Request/GetBusRequest.h Request/GetBusRequest.cpp
        Request/GetStopRequest.h Request/GetStopRequest.cpp

        RequestsReader/RequestsReader.h
        RequestsReader/RequestsStreamReader.h RequestsReader/RequestsStreamReader.cpp
        RequestsReader/RequestsJsonReader.h RequestsReader/RequestsJsonReader.cpp

        RequestVisitor/RequestVisitor.h
        RequestVisitor/RequestComputeBaseVisitor.h RequestVisitor/RequestComputeBaseVisitor.cpp
        RequestVisitor/RequestComputeStatVisitor.h RequestVisitor/RequestComputeStatVisitor.cpp
        RequestVisitor/RequestReadFromStreamVisitor.h RequestVisitor/RequestReadFromStreamVisitor.cpp
        RequestVisitor/RequestReadFromJsonVisitor.h RequestVisitor/RequestReadFromJsonVisitor.cpp

        Response/Response.h
        Response/GetBusResponse.h Response/GetBusResponse.cpp
        Response/GetStopResponse.h Response/GetStopResponse.cpp

        ResponseVisitor/ResponseVisitor.h
        ResponseVisitor/ResponseWriteToStreamVisitor.h ResponseVisitor/ResponseWriteToStreamVisitor.cpp
        ResponseVisitor/ResponseWriteToJsonVisitor.h ResponseVisitor/ResponseWriteToJsonVisitor.cpp

        ResponseWriter/ResponseWriter.h
        ResponseWriter/ResponsesStreamWriter.h ResponseWriter/ResponsesStreamWriter.cpp
        ResponseWriter/ResponsesJsonWriter.h ResponseWriter/ResponsesJsonWriter.cpp

        StopDistancesManager/StopDistancesManager.h StopDistancesManager/StopDistancesManager.cpp

        TransportDirectory/TransportDirectory.h TransportDirectory/TransportDirectory.cpp
        TransportDirectory/TransportDirectoryBuilder.h TransportDirectory/TransportDirectoryBuilder.cpp

        Json/json.h Json/json.cpp)


include_directories(
        ./json
        ./Request
        ./RequestVisitor
        ./RequestsReader
        ./Response
        ./ResponseVisitor
        ./ResponseWriter
        ./StopDistancesManager
        ./TransportDirectory
)

