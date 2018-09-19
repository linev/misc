all: build clean

build:
	cp ~/git/civetweb/src/civetweb.c .
	cp ~/git/civetweb/src/*.inl .
	cp ~/git/civetweb/include/civetweb.h .
	@echo "build with direct linking"
	gcc -Wall  main.c civetweb.c -pthread -ldl -lssl -lcrypto -DUSE_WEBSOCKET -DOPENSSL_API_1_1 -DNO_SSL_DL -o bld_gcc_ssl11
	g++ -Wall  main.c civetweb.c -pthread -ldl -lssl -lcrypto -DUSE_WEBSOCKET -DOPENSSL_API_1_1 -DNO_SSL_DL -o bld_gpp_ssl11
	clang -Wall  main.c civetweb.c -pthread -ldl -lssl -lcrypto -DUSE_WEBSOCKET -DOPENSSL_API_1_1 -DNO_SSL_DL -o bld_clang_ssl11
	gcc -Wall  main.c civetweb.c -pthread -ldl -lssl -lcrypto -DUSE_WEBSOCKET -DNO_SSL_DL -o bld_gcc
	g++ -Wall  main.c civetweb.c -pthread -ldl -lssl -lcrypto -DUSE_WEBSOCKET -DNO_SSL_DL -o bld_gpp
	clang -Wall  main.c civetweb.c -pthread -ldl -lssl -lcrypto -DUSE_WEBSOCKET -DNO_SSL_DL -o bld_clang
	@echo "build with dynamic loading"
	gcc -Wall  main.c civetweb.c -pthread -ldl -DUSE_WEBSOCKET -DOPENSSL_API_1_1 -o bld_gcc_ssl11_dl
	g++ -Wall  main.c civetweb.c -pthread -ldl -DUSE_WEBSOCKET -DOPENSSL_API_1_1 -o bld_gpp_ssl11_dl
	clang -Wall  main.c civetweb.c -pthread -ldl -DUSE_WEBSOCKET -DOPENSSL_API_1_1 -o bld_clang_ssl11_dl
	gcc -Wall  main.c civetweb.c -pthread -ldl -DUSE_WEBSOCKET -o bld_gcc_dl
	g++ -Wall  main.c civetweb.c -pthread -ldl -DUSE_WEBSOCKET -o bld_gpp_dl
	clang -Wall  main.c civetweb.c -pthread -ldl -DUSE_WEBSOCKET -o bld_clang_dl


clean:
	rm -f bld_* *.inl *.h civetweb.c