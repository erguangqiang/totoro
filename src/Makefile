
CPPFLAGS = -Wall -g

all : totoro
totoro : totoro.o ttr_epoll.o ttr_epoll_event.o ttr_local_socket.o ttr_startup.o
	$(CXX) $(CPPFLAGS) $^ -o $@

totoro.o : totoro.cpp
	$(CXX) $(CPPFLAGS) -I../include -c $< 

ttr_epoll.o : ttr_epoll.cpp
	$(CXX) $(CPPFLAGS) -I../include -c $<

ttr_epoll_event.o : ttr_epoll_event.cpp
	$(CXX) $(CPPFLAGS) -I../include -c $<

ttr_local_socket.o : ttr_local_socket.cpp
	$(CXX) $(CPPFLAGS) -I../include -c $<

ttr_startup.o : ttr_startup.cpp
	$(CXX) $(CPPFLAGS) -I../include -c $<

clean:
	rm -rf *.o totoro
