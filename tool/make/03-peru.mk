ALSO += .peru/lastimports .WAIT

.peru/lastimports: peru.yaml
	@echo "Peru Syncing..."
	@peru sync
	@touch $@
