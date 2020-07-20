install:
	@sudo mount -t drvfs g: /mnt/g # Mount sdcard (g drive) in wsl2
	@sudo mv TestPlugin/default.3gx /mnt/g/luma/plugins/default.3gx