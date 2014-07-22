.PHONY: clean All

All:
	@echo ----------Building project:[ ece454a4server - Debug ]----------
	@"$(MAKE)" -f "ece454a4server.mk"
clean:
	@echo ----------Cleaning project:[ ece454a4server - Debug ]----------
	@"$(MAKE)" -f "ece454a4server.mk" clean
