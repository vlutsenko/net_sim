netopeer2-cli

connect

get --filter-xpath /ietf-interfaces:interfaces

edit-config --target running --config=int_ip.xml

get-config --source running --filter-xpath /ietf-interfaces:interfaces

gcc -o int_ext int_ip.c $(pkg-config --cflags --libs libyang sysrepo)
