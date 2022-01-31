cmake_minimum_required(VERSION 3.21)
project(Transport)

set(CMAKE_CXX_STANDARD 20)

add_executable(Transport
        main.cpp

        Libs/graph.h
        Libs/router.h
        Libs/range.h
        Libs/Json/json.h Libs/Json/json.cpp

        RoutingSettings/RoutingSettings.h

        Request/Request.h Request/Request.cpp
        Request/Base/AddBusRequest.h Request/Base/AddBusRequest.cpp
        Request/Base/AddStopRequest.h Request/Base/AddStopRequest.cpp
        Request/Stat/GetBusRequest.h Request/Stat/GetBusRequest.cpp
        Request/Stat/GetStopRequest.h Request/Stat/GetStopRequest.cpp
        Request/Stat/RouteRequest.h Request/Stat/RouteRequest.cpp

        RequestsReader/RequestsReader.h
        RequestsReader/JsonRequestsReader.h RequestsReader/JsonRequestsReader.cpp

        RequestVisitor/RequestVisitor.h
        RequestVisitor/ComputeBaseRequestVisitor.h RequestVisitor/ComputeBaseRequestVisitor.cpp
        RequestVisitor/ComputeStatRequestVisitor.h RequestVisitor/ComputeStatRequestVisitor.cpp
        RequestVisitor/ReadFromJsonRequestVisitor.h RequestVisitor/ReadFromJsonRequestVisitor.cpp

        Response/Response.h
        Response/GetBusResponse.h Response/GetBusResponse.cpp
        Response/GetStopResponse.h Response/GetStopResponse.cpp
        Response/RouteResponse.h Response/RouteResponse.cpp

        ResponseVisitor/ResponseVisitor.h
        ResponseVisitor/WriteToJsonResponseVisitor.h ResponseVisitor/WriteToJsonResponseVisitor.cpp

        ResponseWriter/ResponseWriter.h
        ResponseWriter/JsonResponseWriter.h ResponseWriter/JsonResponseWriter.cpp

        StopDistancesManager/StopDistancesManager.h StopDistancesManager/StopDistancesManager.cpp

        TransportDirectory/TransportDirectory.h TransportDirectory/TransportDirectory.cpp
        TransportDirectory/TransportDirectoryBuilder.h TransportDirectory/TransportDirectoryBuilder.cpp
        TransportDirectory/TransportDirectoryRouteItems.h TransportDirectory/TransportDirectoryRouteItems.cpp)


include_directories(
        ./Libs
        ./Libs/Json
        ./Request
        ./Request/Base
        ./Request/Stat
        ./RequestVisitor
        ./RequestsReader
        ./Response
        ./ResponseVisitor
        ./ResponseWriter
        ./StopDistancesManager
        ./TransportDirectory
        ./RoutingSettings
)

