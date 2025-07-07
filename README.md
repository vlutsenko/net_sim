netopeer2-cli

connect

get --filter-xpath /ietf-interfaces:interfaces

edit-config --target running --config=int_ip.xml

get-config --source running --filter-xpath /ietf-interfaces:interfaces

gcc -o int_ext int_ip.c $(pkg-config --cflags --libs libyang sysrepo)


##### get go the containers 
ubnt@ubuntu:~/yang_models/dzs_olt_fc_sim/dzs_olt_fc_sim$ docker ps 

CONTAINER ID   IMAGE              COMMAND                 CREATED         STATUS         PORTS                                     NAMES

eabf161a4727   netopeer2-server   "netopeer2-server -d"   6 minutes ago   Up 6 minutes   0.0.0.0:830->830/tcp, [::]:830->830/tcp   netconf-server

ubnt@ubuntu:~/yang_models/dzs_olt_fc_sim/dzs_olt_fc_sim$ docker exec -it eabf161a4727 /bin/bash

root@eabf161a4727:/opt# 

