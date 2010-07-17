DLLDIR = dllint
cs: cs.py $(DLLDIR)/dllint.py
	python cs.py
pos: vt.py pos.py $(DLLDIR)/dllint.py
	python vt.py
clean:
	rm *.pyc
	rm $(DLLDIR)/*.pyc
	rm journal.txt
