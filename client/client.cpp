//
// Created by evanlyu732 on 2022/4/3.
//

//TODO

#include "../proto_generation/product.pb.h"
#include "../proto_generation/product.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <map>
#include <utility>
#include <fmt/core.h>

using ecommerce::Product;
using ecommerce::ProductId;
using ecommerce::ProductInfo;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;



