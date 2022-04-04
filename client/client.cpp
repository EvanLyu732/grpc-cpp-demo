//
// Created by evanlyu732 on 2022/4/3.
//

#include "../proto_generation/product.pb.h"
#include "../proto_generation/product.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <fmt/core.h>
#include <map>
#include <random>
#include <utility>
#include <csignal>
#include <thread>

using ecommerce::Product;
using ecommerce::ProductId;
using ecommerce::ProductInfo;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class Client{
public:
    Client(std::shared_ptr<grpc::Channel> channel): _stub(ProductInfo::NewStub(channel))  {};
    ~Client()=default;

    std::shared_ptr<Product> generateRandomProduct() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dis(1, 9999);
        Product pd;
        pd.set_name(std::to_string(dis(rng)));
        pd.set_id(std::to_string(dis(rng)));
        pd.set_description(std::to_string(dis(rng)));
        pd.set_price(dis(rng));
        return std::make_shared<Product>(std::move(pd));
    }


    //TODO
    bool addProduct(){
        return true;
    }


    //TODO
    bool getProduct(){
        return true;
    }

private:
    std::unique_ptr<ProductInfo::Stub> _stub;
};


void signalHandler(int sig) {
    std::cout << "Client stop running!" << '\n';
}

auto initialSignalHandler() {
    signal(SIGTERM, signalHandler);
}


auto main() -> int
{
    Client client{grpc::CreateChannel("127.0.0.1:5001", grpc::InsecureChannelCredentials())};


}