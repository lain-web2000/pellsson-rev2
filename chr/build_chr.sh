set -e
python genchr.py

cat smb/smborg-sprites.chr smb/smborg-back.chr smborg-lost-charset.chr \
	smb/lost-sprites.chr smb/lost-back.chr smb/lost-victory.chr lost-smborg-charset.chr \
	smb/nippon-sprites.chr smb/nippon-back.chr smb/nippon-victory.chr nippon-smborg-charset.chr \
	smb/peach-sprites.chr \
	intro/intro-bg.chr intro-sprbank0.chr intro-sprbank0.chr intro-sprbank0.chr > full.chr

