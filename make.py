#!/usr/bin/python
import paramiko 
import subprocess
import sys
import re

host = '192.168.0.254'
user = 'root'
password = 'toor'
port = 22
remotepathWww = "/Server/www/"
remotepathServer = "/Server/"
localpathWww = "./Server_new/www/"
localpathServer = "./Server_new/"


if len (sys.argv) < 2:
	print ("Ошибка. Выбери один из параметров:")
	print ("  wasm  - собрать DOORS.wasm")
	print ("  srv   - собрать сервер DOORS")
	print ("  start - запустить сервер DOORS")
	print ("  stop  - остановить сервер DOORS")
	print ("  state - узнать запущен ли сервер DOORS")
	sys.exit (1)
if len (sys.argv) > 3:
	print ("Ошибка. Выбери один из параметров:")
	print ("  wasm  - собрать DOORS.wasm")
	print ("  srv   - собрать сервер DOORS")
	print ("  start - запустить сервер DOORS")
	print ("  stop  - остановить сервер DOORS")
	print ("  state - узнать запущен ли сервер DOORS")
	sys.exit (1)

if (sys.argv[1] == "wasm"):
	# MakeFile
	subprocess.run(["make"])
	print("Make   WASM OK")
	
	# Upload file to server
	print("Upload WASM", end =" ")
	transport = paramiko.Transport((host, port))
	transport.connect(username = user, password = password)
	sftp = paramiko.SFTPClient.from_transport(transport)

	#sftp.get(remotepath, localpath)
	sftp.put(localpathWww + "DOORS.wasm", remotepathWww + "DOORS.wasm")
	sftp.put(localpathWww + "index.html", remotepathWww + "index.html")

	sftp.close()
	transport.close()
	del transport, sftp 
	print("OK")
	
elif (sys.argv[1] == "srv"):
	# MakeFile
	subprocess.run(["make", "-C", localpathServer])
	print("Make   SERVER OK")
	
	# Connect SSH
	ssh = paramiko.SSHClient()
	ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
	ssh.connect(hostname = host, username = user, password = password, port = port)
	
	# Killall DOORS
	stdin, stdout, stderr = ssh.exec_command("killall DOORS")
	print("Stop   SERVER OK")
	
	# Upload file to server
	transport = paramiko.Transport((host, port))
	transport.connect(username = user, password = password)
	sftp = paramiko.SFTPClient.from_transport(transport)

	#sftp.get(remotepath, localpath)
	sftp.put(localpathServer + "DOORS", remotepathServer + "DOORS")

	sftp.close()
	transport.close()
	del transport, sftp 
	print("Upload SERVER OK")
	
	# Chmod +x DOORS
	stdin, stdout, stderr = ssh.exec_command("cd " + remotepathServer)
	stdin, stdout, stderr = ssh.exec_command("chmod +x DOORS")
	#print(stdout.read().decode())
	#print("Chmod  SERVER OK")

	stdin, stdout, stderr = ssh.exec_command("/root/startDOORS.sh > /dev/null 2>&1")
	print("Start  SERVER OK")
	
	ssh.close()
	del ssh, stdin, stdout, stderr
	
elif (sys.argv[1] == "start"):
	# Connect SSH
	ssh = paramiko.SSHClient()
	ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
	ssh.connect(hostname = host, username = user, password = password, port = port)
	
	# Killall DOORS
	stdin, stdout, stderr = ssh.exec_command("killall DOORS")
	print("Stop   SERVER OK")
	
	# Chmod +x DOORS
	stdin, stdout, stderr = ssh.exec_command("cd " + remotepathServer)
	stdin, stdout, stderr = ssh.exec_command("chmod +x DOORS")
	#print(stdout.read().decode())
	#print("Chmod  SERVER OK")

	stdin, stdout, stderr = ssh.exec_command("/root/startDOORS.sh > /dev/null 2>&1")
	print("Start  SERVER OK")
	
	ssh.close()
	del ssh, stdin, stdout, stderr
	
elif (sys.argv[1] == "stop"):
	# Connect SSH
	ssh = paramiko.SSHClient()
	ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
	ssh.connect(hostname = host, username = user, password = password, port = port)
	
	# Killall DOORS
	stdin, stdout, stderr = ssh.exec_command("killall DOORS")
	print("Stop   SERVER OK")
	
	ssh.close()
	del ssh, stdin, stdout, stderr
	
elif (sys.argv[1] == "state"):
	# Connect SSH
	ssh = paramiko.SSHClient()
	ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
	ssh.connect(hostname = host, username = user, password = password, port = port)
	
	# Killall DOORS
	stdin, stdout, stderr = ssh.exec_command("ps | grep DOORS")
	strings = stdout.read().decode().split('\n')  
	
	isRun = False
	for str in strings:
		str = re.sub(r'\s+', ' ', str)
		str = str.strip()
		words = str.split(' ')
		try:
			if words[4] == "./DOORS":
				print("Server is RUN")
				isRun = True
		except:
			pass
	if isRun != True:
		 print("Server is STOP")
			
	ssh.close()
	del ssh, stdin, stdout, stderr
    
if (sys.argv[1] == "git"):
    # Git commit to GitHub
    subprocess.run(["git", "add", "."])
    print("OK: git add .")
    comment = sys.argv[2]
    subprocess.run(["git", "commit", "-m", comment])
    print("OK: git commit -m \"{0}\"".format(comment))
	
	
else:
	print ("Ошибка. Выбери один из параметров:")
	print ("  wasm  - собрать DOORS.wasm")
	print ("  srv   - собрать сервер DOORS")
	print ("  start - запустить сервер DOORS")
	print ("  stop  - остановить сервер DOORS")
	print ("  state - узнать запущен ли сервер DOORS")
	sys.exit (1)
	
	

	


