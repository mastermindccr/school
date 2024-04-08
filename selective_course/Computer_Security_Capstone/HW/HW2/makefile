all:
	# generate certificate
	openssl genrsa -out ca.key 2048
	openssl req -new -x509 -days 1000 -key ca.key -out ca.crt -subj "/CN=attacker"
	# open ip forwarding to forward the victims' packets to the gateway
	sudo sysctl -w net.ipv4.ip_forward=1
	cp mitm_attack.py mitm_attack
	chmod +x mitm_attack
	cp pharm_attack.py pharm_attack
	chmod +x pharm_attack
clean:
	sudo rm ca.key ca.crt logfile mitm_attack pharm_attack