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

### to check if input (in xml) fits to the yang models
yanglint -i -t config -m ietf-system@2014-08-06.yang dzs-system-deviations@2024-06-21.yang usr.xml
yanglint -p /opt/modules/ -i -t config \
  -m ietf-system@2014-08-06.yang dzs-system-deviations.yang usr.xml

### tree representation of data structure after models consolidation
yanglint -f tree ietf-system@2014-08-06.yang dzs-system-deviations.yang
