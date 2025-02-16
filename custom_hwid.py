Import("env")

print(env.Dump())

board_config = env.BoardConfig()

# should be array of VID:PID pairs
board_config.update("build.hwids", [
  ["0x1209", "0x0001"]
])

board_config.update("build.usb_product", "DIY Arduino Button Box")