#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysrepo.h>
#include <libyang/libyang.h>

// Operational data callback
static int
interfaces_oper_cb(sr_session_ctx_t *session, uint32_t sub_id, const char *module_name,
                   const char *path, const char *request_xpath, uint32_t request_id,
                   struct lyd_node **parent, void *private_data)
{
    const struct ly_ctx *ctx = sr_session_acquire_context(session);
    struct lyd_node *interfaces = NULL;
    char *interface_name = "eth0";
    char *interface_type = "iana-if-type:ethernetCsmacd";
    char *oper_status = "up";
    char *ipv4_address = "10.2.1.1";
    char *ipv4_prefix = "24";
    char *port_layer_if = "eth0-port";
    uint32_t speed_mbps = 1000; // speed in Mbps

    char xpath[256];

    // Create root container /interfaces
    if (lyd_new_path(NULL, ctx, "/ietf-interfaces:interfaces", NULL, 0, &interfaces) != LY_SUCCESS) {
        fprintf(stderr, "Failed to create /ietf-interfaces:interfaces\n");
        return SR_ERR_INTERNAL;
    }

    // Set interface name
    snprintf(xpath, sizeof(xpath), "/ietf-interfaces:interfaces/interface[name='%s']/name", interface_name);
    lyd_new_path(interfaces, ctx, xpath, interface_name, 0, NULL);

    // Set type
    snprintf(xpath, sizeof(xpath), "/ietf-interfaces:interfaces/interface[name='%s']/ietf-interfaces:type", interface_name);
    lyd_new_path(interfaces, ctx, xpath, interface_type, 0, NULL);

    // Set oper-status
    snprintf(xpath, sizeof(xpath), "/ietf-interfaces:interfaces/interface[name='%s']/ietf-interfaces:oper-status", interface_name);
    lyd_new_path(interfaces, ctx, xpath, oper_status, 0, NULL);

    // Add IP address (ietf-ip)
    snprintf(xpath, sizeof(xpath),
             "/ietf-interfaces:interfaces/interface[name='%s']/ietf-ip:ipv4/ietf-ip:address[ietf-ip:ip='%s']/ietf-ip:ip",
             interface_name, ipv4_address);
    lyd_new_path(interfaces, ctx, xpath, ipv4_address, 0, NULL);

    snprintf(xpath, sizeof(xpath),
             "/ietf-interfaces:interfaces/interface[name='%s']/ietf-ip:ipv4/ietf-ip:address[ietf-ip:ip='%s']/ietf-ip:prefix-length",
             interface_name, ipv4_address);
    lyd_new_path(interfaces, ctx, xpath, ipv4_prefix, 0, NULL);

    // Add bbf-hardware augmentation (port-layer-if)
    snprintf(xpath, sizeof(xpath),
             "/ietf-interfaces:interfaces/interface[name='%s']/bbf-hardware:hardware-component/bbf-hardware:port-layer-if",
             interface_name);
    lyd_new_path(interfaces, ctx, xpath, port_layer_if, 0, NULL);

    // Add interface speed (example custom or augmented leaf)
    snprintf(xpath, sizeof(xpath),
             "/ietf-interfaces:interfaces/interface[name='%s']/speed", interface_name);
    char speed_str[32];
    snprintf(speed_str, sizeof(speed_str), "%u", speed_mbps);
    lyd_new_path(interfaces, ctx, xpath, speed_str, 0, NULL);

    *parent = interfaces;
    return SR_ERR_OK;
}

int main(void)
{
    sr_conn_ctx_t *connection = NULL;
    sr_session_ctx_t *session = NULL;
    sr_subscription_ctx_t *subscription = NULL;
    int rc = SR_ERR_OK;

    // Connect to sysrepo
    rc = sr_connect(SR_CONN_DEFAULT, &connection);
    if (rc != SR_ERR_OK) {
        fprintf(stderr, "Error connecting to sysrepo: %s\n", sr_strerror(rc));
        return rc;
    }

    // Start a session
    rc = sr_session_start(connection, SR_DS_RUNNING, &session);
    if (rc != SR_ERR_OK) {
        fprintf(stderr, "Error starting session: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    // Subscribe to operational data for ietf-interfaces
    rc = sr_oper_get_subscribe(session, "ietf-interfaces", "/ietf-interfaces:interfaces",
                               interfaces_oper_cb, NULL, 0, &subscription);
    if (rc != SR_ERR_OK) {
        fprintf(stderr, "Operational subscribe failed: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    printf("Provider for ietf-interfaces with speed support is running.\n");
    printf("Press ENTER to stop.\n");
    getchar();

cleanup:
    sr_unsubscribe(subscription);
    sr_session_stop(session);
    sr_disconnect(connection);
    return rc;
}
