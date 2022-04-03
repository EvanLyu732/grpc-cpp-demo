//
// Created by evanlyu732 on 2022/4/3.
//
#include "../proto_generation/product.pb.h"
#include "../proto_generation/product.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <map>
#include <utility>
#include <fmt/core.h>
#include <csignal>
#include <iostream>

using ecommerce::Product;
using ecommerce::ProductId;
using ecommerce::ProductInfo;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class ServerServiceImpl final: public ProductInfo::Service{
public:
    Status addProduct(ServerContext* context, const Product* request, ProductId* response){
        fmt::print("server receives add product call...");
        ProductId id;
        id.set_value(productId);
        Product* product{std::move(const_cast<Product*>(request))};
        std::cout << "product map ready to emplace back product, product id:" << productId << '\n';
        productMap.insert(std::make_pair(&id, product));
        return Status::OK;
    }

    Status getProduct(ServerContext* context, const ProductId* request, Product* response){
        fmt::print("server receives get product call...");
        try{
            Product* product = productMap.at(const_cast<ProductId*>(request));
            response = product;
            return Status::OK;
        }
        catch (const std::out_of_range&) {
            std::cout << "product is not in map..., product id{}" << request->value() << '\n';
        }
        return Status::OK;
    }

private:
    std::map<ProductId*, Product*> productMap;
    uint32_t productId;
};

void signalHandler(int sig) {
    std::cout << "Server stop running!" << '\n';
}

auto initialSignalHandler() {
    signal(SIGTERM, signalHandler);
}


auto main() -> int{
    std::cout << "into  main" << '\n';
    initialSignalHandler();
    std::cout << "Prduct Server start running..." << '\n';
    std::string address{"127.0.0.1:50051"};
    ServerServiceImpl serve;
    ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&serve);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    server->Wait();
    return 0;
}
