#!/usr/bin/python3
import subprocess
import sys
import secret


userGit = "GorComComputing"
repoGit = "TFT_Terminal"


if len (sys.argv) < 2:
	print ("Error. Choose one from parameters:")
	print ("  git  - commit to GitHub")
	sys.exit (1)
if len (sys.argv) > 3:
	print ("Error. Choose one from parameters:")
	print ("  git  - commit to GitHub")
	sys.exit (1)

    
if (sys.argv[1] == "git"):
    # Git commit to GitHub
    subprocess.run(["git", "add", "."])
    print("OK: git add .")
    
    if len (sys.argv) < 3:
        comment = "-//-"
    else:
        comment = sys.argv[2]
    subprocess.run(["git", "commit", "-m", comment])
    print("OK: git commit -m \"{0}\"".format(comment))
    
    subprocess.run(["git", "push", "https://{0}@github.com/{1}/{2}.git".format(secret.tokenGit, userGit, repoGit)])
    print("OK: git push to {0}".format(repoGit))
    
	
else:
	print ("Error. Choose one from parameters:")
	print ("  git  - commit to GitHub")
	sys.exit (1)
	
	

	


