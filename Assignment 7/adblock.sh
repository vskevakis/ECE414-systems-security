#!/bin/bash
# You are NOT allowed to change the files' names!
domainNames="domainNames.txt"
IPAddresses="IPAddresses.txt"
adblockRules="adblockRules"

function adBlock() {
    if [ "$EUID" -ne 0 ];then
        printf "Please run as root.\n"
        exit 1
    fi
    if [ "$1" = "-domains"  ]; then
        # Find IPs of domains
        # Setting input file
        input=${domainNames}
        # Read file line by line
        while IFS= read -r line; do
            # Use dig to fetch ip address of domain
            ips="$(dig +short "$line")"
            # Parse through found ip addresses
            while IFS= read -r ip_address; do
                # Check if its a valid ip address
                if [[ $ip_address =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
                    # Add rule
                    sudo iptables -A OUTPUT -d ${ip_address} -j REJECT
                fi                
            done <<< "$ips"
        done < "$input"
        true
            
    elif [ "$1" = "-ips"  ]; then
        # Configure adblock rules based on the IP addresses of $IPAddresses file.
        # Write your code here...
        # Create firewall rules from IPAddresses
        # Setting input file
        input=${IPAddresses}
        # Read file line by line
        while IFS= read -r ip_address; do
            # Check if its a valid ip address
            if [[ $ip_address =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
                # Add rule
                sudo iptables -A OUTPUT -d ${ip_address} -j REJECT
            fi
        done < "$input"
        true
        
    elif [ "$1" = "-save"  ]; then
        # Save rules to $adblockRules file.
        # Write your code here...
        iptables-save > ${adblockRules}
        true
        
    elif [ "$1" = "-load"  ]; then
        # Load rules from $adblockRules file.
        # Write your code here...
        iptables-restore < ${adblockRules}
        true

        
    elif [ "$1" = "-reset"  ]; then
        # Reset rules to default settings (i.e. accept all).
        # Write your code here...
        iptables -P INPUT ACCEPT &&
        iptables -P FORWARD ACCEPT &&
        iptables -P OUTPUT ACCEPT &&
        iptables -t nat -F &&
        iptables -t mangle -F &&
        iptables -F &&
        iptables -X &&
        true

        
    elif [ "$1" = "-list"  ]; then
        # List current rules.
        # Write your code here...
        iptables -L -nv
        true
        
    elif [ "$1" = "-help"  ]; then
        printf "This script is responsible for creating a simple adblock mechanism. It rejects connections from specific domain names or IP addresses using iptables.\n\n"
        printf "Usage: $0  [OPTION]\n\n"
        printf "Options:\n\n"
        printf "  -domains\t  Configure adblock rules based on the domain names of '$domainNames' file.\n"
        printf "  -ips\t\t  Configure adblock rules based on the IP addresses of '$IPAddresses' file.\n"
        printf "  -save\t\t  Save rules to '$adblockRules' file.\n"
        printf "  -load\t\t  Load rules from '$adblockRules' file.\n"
        printf "  -list\t\t  List current rules.\n"
        printf "  -reset\t  Reset rules to default settings (i.e. accept all).\n"
        printf "  -help\t\t  Display this help and exit.\n"
        exit 0
    else
        printf "Wrong argument. Exiting...\n"
        exit 1
    fi
}

adBlock $1
exit 0