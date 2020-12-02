# Configure app environment
# Set this explicitly
$api_key = "a35a2a72bd230ac0aa0f52715bbdc6aa"

AfterConfiguration do |_config|
  MazeRunner.config.receive_no_requests_wait = 10 if MazeRunner.config.respond_to? :receive_no_requests_wait=
  # TODO: Remove once the Bugsnag-Integrity header has been implemented
  MazeRunner.config.enforce_bugsnag_integrity = false if MazeRunner.config.respond_to? :enforce_bugsnag_integrity=
end

Before('@skip') do |scenario|
  skip_this_scenario("Skipping scenario")
end

Before('@skip_above_android_8') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version >= 9
end

Before('@skip_above_android_7') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version >= 8
end

Before('@skip_below_android_9') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version < 9
end

Before('@skip_below_android_8') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version < 8
end

Before('@skip_android_8_1') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version == 8.1
end

Before('@skip_android_10') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version == 10
end

Before('@skip_android_11') do |scenario|
  skip_this_scenario("Skipping scenario") if MazeRunner.config.os_version == 11
end
