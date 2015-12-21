import http.client
import json
import hashlib

LOGIN_INIT = 0
LOGOUT = 1
GET_ALL_RULES = 2
DELETE_RULE = 3
BLOCK_IP = 4
BLOCK_TCP_PORT = 5
BLOCK_UDP_PORT = 6
BLOCK_INCOMING_MAC = 7
RAW = 8


def get_user_command():
    return int(input(
        "Available commands:\n1 - Logout\n2 - Get all rules\n3 - Delete rule\n4 - Block IP\n5 - Block TCP port\n6 - Block UDP port\n7 - Block incoming MAC\n8 - Raw\nCommand: "))


def get_chain_type():
    return int(input("ChainType (0 - INPUT, 1 - OUTPUT): "))


def get_path():
    return '/iptables_mgmt'


def get_username_password():
    return 'mareczek', '123456'


def get_host():
    return 'localhost'


def get_port():
    return '12345'


def send_json_http_request(host, port, path, json_data):
    headers = {'Content-type': 'application/json'}

    connection = http.client.HTTPConnection(host + ":" + port)
    connection.request('POST', path, json_data, headers)

    response = connection.getresponse()
    return response.read().decode()


def create_login_init_json(username):
    data = {'command': LOGIN_INIT, 'hash': '', 'challenge': '', 'params': {'username': username}}
    return json.dumps(data)


def create_logout_json(challenge, md5hash):
    data = {'command': LOGOUT, 'hash': md5hash, 'challenge': challenge, 'params': {}}
    return json.dumps(data)


def create_get_all_rules_json(challenge, md5hash):
    data = {'command': GET_ALL_RULES, 'hash': md5hash, 'challenge': challenge, 'params': {}}
    return json.dumps(data)


def create_delete_rule_json(challenge, md5hash, chain_type, line):
    data = {'command': DELETE_RULE, 'hash': md5hash, 'challenge': challenge,
            'params': {'chainType': chain_type, 'line': line}}
    return json.dumps(data)


def create_block_ip_json(challenge, md5hash, chain_type, ip):
    data = {'command': BLOCK_IP, 'hash': md5hash, 'challenge': challenge,
            'params': {'chainType': chain_type, 'ip': ip}}
    return json.dumps(data)


def create_block_tcp_port_json(challenge, md5hash, chain_type, tcp_port):
    data = {'command': BLOCK_TCP_PORT, 'hash': md5hash, 'challenge': challenge,
            'params': {'chainType': chain_type, 'tcpPort': tcp_port}}
    return json.dumps(data)


def create_block_udp_port_json(challenge, md5hash, chain_type, udp_port):
    data = {'command': BLOCK_UDP_PORT, 'hash': md5hash, 'challenge': challenge,
            'params': {'chainType': chain_type, 'udp_port': udp_port}}
    return json.dumps(data)


def create_block_incoming_mac_json(challenge, md5hash, mac):
    data = {'command': BLOCK_INCOMING_MAC, 'hash': md5hash, 'challenge': challenge, 'params': {'mac': mac}}
    return json.dumps(data)


def create_raw_json(challenge, md5hash, raw):
    data = {'command': RAW, 'hash': md5hash, 'challenge': challenge, 'params': {'raw': raw}}
    return json.dumps(data)


def calculate_md5(data):
    return hashlib.md5(data.encode('utf-8')).hexdigest()


def main():
    username, password = get_username_password()
    host = get_host()
    port = get_port()
    path = get_path()

    json_data = create_login_init_json(username)

    response = send_json_http_request(host, port, path, json_data)
    response = json.loads(response)

    if response['error_code'] != 0:
        # Probably invalid username

        print("Error " + response['error_message'])
        exit()

    while True:
        command = get_user_command()

        challenge = response['challenge']
        md5hash = calculate_md5(password + challenge)

        if command == LOGOUT:
            json_data = create_logout_json(challenge, md5hash)
        elif command == GET_ALL_RULES:
            json_data = create_get_all_rules_json(challenge, md5hash)
        elif command == DELETE_RULE:
            chain_type = get_chain_type()
            line = int(input("Line to delete: "))
            json_data = create_delete_rule_json(challenge, md5hash, chain_type, line)
        elif command == BLOCK_IP:
            chain_type = get_chain_type()
            ip = input("IP: ")
            json_data = create_block_ip_json(challenge, md5hash, chain_type, ip)
        elif command == BLOCK_TCP_PORT:
            chain_type = get_chain_type()
            tcp_port = int(input("TCP port: "))
            json_data = create_block_tcp_port_json(challenge, md5hash, chain_type, tcp_port)
        elif command == BLOCK_UDP_PORT:
            chain_type = get_chain_type()
            udp_port = int(input("UDP port: "))
            json_data = create_block_udp_port_json(challenge, md5hash, chain_type, udp_port)
        elif command == BLOCK_INCOMING_MAC:
            mac = input("Mac: ")
            json_data = create_block_incoming_mac_json(challenge, md5hash, mac)
        elif command == RAW:
            raw = input("Raw: ")
            json_data = create_raw_json(challenge, md5hash, raw)
        else:
            print("Invalid command")
            continue

        response = send_json_http_request(host, port, path, json_data)
        response = json.loads(response)

        print(str(response['error_code']) + ' ' + response['error_message'])

        if 'data' in response.keys():
            print("Data: " + response['data'])

        if command == LOGOUT:
            exit()


main()