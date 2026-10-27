if(CONFIG_L6TASK1_ON_BOARD_LED_DRIVER)
  target_sources(app PRIVATE l6task1_on_board_led_driver.c)

  # Conditionally compile based on the Kconfig choice
  if(CONFIG_L7_TASK1_SHELL)
    target_sources(app PRIVATE l7task1_on_board_led_shell.c)
  elseif(CONFIG_L7_TASK2_SHELL)
    target_sources(app PRIVATE l7task2_on_board_led_shell.c)
  endif()

  endif()