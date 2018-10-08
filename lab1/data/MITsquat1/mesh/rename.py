import glob
import os

files = glob.glob('*')[:-1]
form = 'mesh_{:04d}.obj'
cnt = 0
for i in range(len(files)):
    src = files[i]
    dst = form.format(cnt)
    os.rename(src, dst)
    cnt += 1
