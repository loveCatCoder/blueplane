# blueplane


### onvif
ip,port,username,password

### rtsp
url,username,password

### 海康
ip,port,username,password

### 大华
ip,port,username,password



## 编译proto

export PATH=/home/zhangning/blueplane/bin/:$$PATH;
protoc --cpp_out=/home/zhangning/blueplane/api --grpc_out=/home/zhangning/blueplane/api --plugin=protoc-gen-grpc=/home/zhangning/blueplane/bin/grpc_cpp_plugin --proto_path=/home/zhangning/blueplane/proto *.proto

protoc  --go_out=/home/zhangning/blueplane/api --go_opt=paths=source_relative --go-grpc_out=/home/zhangning/blueplane/api --go-grpc_opt=paths=source_relative --proto_path=/home/zhangning/blueplane/proto *.proto