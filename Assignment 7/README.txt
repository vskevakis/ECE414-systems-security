Skevakis Vasileios
AM: 2012030033

ACE414 - Exercise 7 - Report

-domains
	> We read the file with the domains line by line and for each line (domain) and used dig +short to fetch the ip addresses of each domain.
	> After fetching each IP address (One domain can have multiple addresses) we use the rule <<-A OUTPUT -d {IP} -j REJECT
	to reject all outgoing connections to each IP so they don't serve us ads.
	
-ips
	> We read the file with the ips line by line and for each line (ip) we use the rule <<-A OUTPUT -d {IP} -j REJECT to reject all outgoing connections to each IP so they don't serve us ads.
	
-save
	> We save the current rules to the appropriate file by using iptables specific command iptables-save.
	
-load
	> We load the rules from the adblockRules file by using the iptables command iptables-restore
	
-reset
	> We reset all the current iptables rules with multiple commands (even though we really need only the iptables -P OUTPUT ACCEPT, to accept all the outgoing connections)

-list 
	> We use iptables -L -nv to list all the current iptables rules.