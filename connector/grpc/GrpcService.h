#ifndef _BLUEPLANE_CONNECTOR_GRPCSERVICE_H_
#define _BLUEPLANE_CONNECTOR_GRPCSERVICE_H_



#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "api/producer.grpc.pb.h"

using pbproducer::Producer;

class GrpcService final : public Producer::Service
{


};























#endif

