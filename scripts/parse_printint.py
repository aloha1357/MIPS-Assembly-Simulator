import re
lines=open('tmp_clean.err','r',encoding='utf-8',errors='ignore').read().splitlines()
prints=[]
for ln in lines:
    m=re.search(r'TRACE: Cpu::printInt pc=(\d+) value=(.*)',ln)
    if m:
        prints.append((int(m.group(1)),m.group(2)))
print('found',len(prints),'printInt entries')
for i,(pc,val) in enumerate(prints, start=1):
    print(f"{i}: pc={pc} value={val}")
for idx in (38,39,52,55,57):
    if idx<=len(prints):
        pc,val=prints[idx-1]
        print(f"TARGET LINE {idx}: pc={pc} value={val}")
    else:
        print(f"TARGET LINE {idx}: no entry (only {len(prints)})")
