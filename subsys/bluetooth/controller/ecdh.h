#include <stdint.h>

#include <bluetooth/hci.h>

void hci_ecdh_init(void);

uint8_t hci_cmd_le_read_local_p256_public_key(void);

uint8_t hci_cmd_le_generate_dhkey(struct bt_hci_cp_le_generate_dhkey *p_params);

uint8_t hci_cmd_le_generate_dhkey_v2(struct bt_hci_cp_le_generate_dhkey_v2 *p_params);
