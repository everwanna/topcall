import tarfile
import zipfile

tar = tarfile.open("media.tar", "w")
tar.add("include", arcname="include")
tar.add("packages", arcname="packages")
tar.add("depends", arcname="depends")
tar.close()
