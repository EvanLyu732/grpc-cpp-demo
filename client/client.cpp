//
// Created by evanlyu732 on 2022/4/3.
//

#include "../proto_generation/product.pb.h"
#include "../proto_generation/product.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <fmt/core.h>
#include <random>
#include <chrono>
#include <utility>
#include <csignal>
#include <thread>

using ecommerce::Product;
using ecommerce::ProductId;
using ecommerce::ProductInfo;

using grpc::ClientReader;
using grpc::ClientContext;
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
        std::shared_ptr<Product> product = this->generateRandomProduct();
        const Product request = std::as_const(*product);
        ClientContext context;
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(5));
        std::cout << "Product id: " << request.id() << '\n';
        ProductId* response{};
        Status status = _stub->addProduct(&context, request, response);
        std::cout << "status code is: " << status.error_code() << '\n';
        if (status.ok()){
            std::cout << "add product successed, product id:" << request.id() << '\n';
            return true;
        } else {
            std::cout << "add product failed" << '\n';
            return false;
        }
    }


    //TODO
    [[deprecated()]]
    bool getProduct(){
        return true;
    }

private:
    std::unique_ptr<ProductInfo::Stub> _stub;
};


void signalHandler(int sig) {
    std::cout << "Client stop running!" << '\n';
    std::terminate();
}

auto initialSignalHandler() {
    signal(SIGINT, signalHandler);
}


auto main() -> int {
    Client client{grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials())};
    std::thread([&]() {
        while (1) {
            client.addProduct();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }).join();
    return 0;
}
