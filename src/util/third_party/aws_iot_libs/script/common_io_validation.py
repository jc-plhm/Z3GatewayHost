common_io_modules = [
  'iot_adc',
  'iot_flash',
  'iot_i2c',
  'iot_pwm',
  'iot_rtc',
  'iot_spi',
  'iot_timer',
  'iot_uart',
  'iot_watchdog'
]

def validate(project):

  for module in common_io_modules:

    if (project.is_selected(module)):

      component = project.component(module)

      if component is not None:

        instances = component.instances();

        if len(instances) > 0:

          config_objs  = []

          for instance in instances:

            config_name = module.upper() + '_CFG_' + instance.upper() + '_INST_NUM'
            config_obj  = project.config(config_name)
            config_objs.append(config_obj)

          config_objs.sort(key=lambda config_obj: int(config_obj.value()))

          if int(config_objs[0].value()) < 0:
            faulty_configs = []

            for config_obj in config_objs:
              if int(config_obj.value()) < 0:
                faulty_configs.append(config_obj.id())

            project.error("Instance numbers for instances of module " + \
                          module + " cannot be negative",
                          project.target_for_defines(faulty_configs),
                          "Instance numbers should be sequential, like 0, 1, 2, ...")

          elif int(config_objs[0].value()) != 0:

            faulty_configs = []

            for config_obj in config_objs:
              faulty_configs.append(config_obj.id())

            project.error("There is no instance for " + \
                          module + " that is configured with INST_NUM of 0",
                          project.target_for_defines(faulty_configs),
                          "Instance numbers should be sequential, like 0, 1, 2, ...")

          else:

            for i in range(0, len(config_objs)-1):

              cur_config_obj = config_objs[i];
              next_config_obj = config_objs[i+1];

              if int(next_config_obj.value()) == int(cur_config_obj.value()):

                  faulty_configs = []

                  faulty_configs.append(cur_config_obj.id())
                  faulty_configs.append(next_config_obj.id())

                  project.error("Module " + module + " has two instances having " + \
                                "the same instance number",
                                project.target_for_defines(faulty_configs),
                                "Instance numbers should be sequential, like 0, 1, 2, ...")

              elif int(next_config_obj.value()) != int(cur_config_obj.value()) + 1:

                  faulty_configs = []

                  faulty_configs.append(cur_config_obj.id())
                  faulty_configs.append(next_config_obj.id())

                  project.error("Module " + module + " has instances with numbers " + \
                                cur_config_obj.value() + " and " + next_config_obj.value() + \
                                ", yet there are no instances between them",
                                project.target_for_defines(faulty_configs),
                                "Instance numbers should be sequential, like 0, 1, 2, ...")
