# Introduction
Service Discovery and Routing Protocol for Mobile ad hoc Networks. SDRP is built as a library and can be deployed in a variety of ways. It was designed to operate both in simulation and on real networks using a simple callback interface to interact with the underlying system.

SDRP is designed to facilitate low-overhead service discovery and routing. Package propagation is minimized through the use of a novel service description approach. See [Below Cross Layer: An Alternative Approach to Service Discovery in MANETs](http://www.tara.tcd.ie/handle/2262/66988) for more details.

# Building

SDRP relies on [Premake](http://premake.github.io/); a build tool that automates the creation of per-platform build scripts based on a LUA configuration file. Download and install premake, then navigate to the build directory and generate build files for your preferred IDE/platform. SDRP is configured to be built as a static library, but you can modify this easily by changing the build script build/premake4.lua.

# Integrating

Implement the SDRPDelegate interface in your own application, then create an SDRP object and register your delegate against it. Register the local node's address and services using the SDRP::LocalNode() methods. Pass incoming packets from other SDRP instances to the local one through the RoutingManager using the HandlePacket. Retrieve the RoutingManager object through SDRP::Routing(). 

In order to maintain a map of the network, nodes should send beacons and advertisements periodically. Call RoutingManager::SendBeacon() and RoutingManager::SendAdvertisement() to trigger this. 
