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

### monify netconf port
sysrepocfg --edit /opt/srv_port.xml --format=xml --datastore=startup --module=ietf-netconf-server

<netconf-server xmlns="urn:ietf:params:xml:ns:yang:ietf-netconf-server">
  <listen>
    <endpoints>
     <endpoint>
        <name>default-ssh</name>
        <ssh>
          <tcp-server-parameters>
            <local-address>0.0.0.0</local-address>
            <local-port>53000</local-port>
          </tcp-server-parameters>
          <ssh-server-parameters>
            <server-identity>
              <host-key>
               <name>default-key</name>
                <public-key>
                  <central-keystore-reference>genkey</central-keystore-reference>
                </public-key>
              </host-key>
              <banner xmlns="urn:cesnet:libnetconf2-netconf-server">netopeer2-netconf-server-2.4.1</banner>
            </server-identity>
            <client-authentication>
              <users>
                <user>
                  <name>root</name>
                  <keyboard-interactive xmlns="urn:cesnet:libnetconf2-netconf-server">
                    <use-system-auth/>
                  </keyboard-interactive>
                </user>
              </users>
            </client-authentication>
          </ssh-server-parameters>
        </ssh>
      </endpoint>
    </endpoints>
  </listen>
</netconf-server>


### tree representation of data structure after models consolidation
yanglint -f tree ietf-system@2014-08-06.yang dzs-system-deviations.yang


### enable features
sysrepoctl -i ietf-system\@2014-08-06.yang -e authentication -e local-users

###
sudo docker rm -f netconf-server && sudo docker build -t netopeer2-server . && sudo docker run -d --name netconf-server -p 830:830 -p 53000:53000 --privileged netopeer2-server && sudo docker exec -it netconf-server /bin/bash
