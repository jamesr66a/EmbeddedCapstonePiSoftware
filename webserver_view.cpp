#include "webserver_model.h"
#include "webserver_view.h"

#include "astar_model_public.h"
#include "encoders_model_public.h"
#include "errorcheck_model_public.h"
#include "motor_model_public.h"
#include "pid_model_public.h"
#include "pose_model_public.h"
#include "rssi_model_public.h"
#include "sensors_model_public.h"
#include "uart_transmitter_public.h"
#include "warning_model_public.h"

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <iostream>
#include <regex>
#include <string>

std::string task_names[] = { "Sensor 1", "RSSI Data Collector", "Encoder 1",
                             "Encoder 2", "Rover Pose Calculator",
                             "PID Controller", "Motor 1", "Motor 2", "UART RX",
                             "UART TX", "Proto Messages", "ErrorCheck",
                             "Warnings", "Pose Calculator", "Control Task" };

std::vector<std::string> event_names[] = {
  { "Receive", "Send", "Left Sensor Value", "Right Sensor Value",
    "Center Sensor Value" },                              // sensor 1
  { "Receive", "Buffer Overrun", "Total Messages Rxed" }, // RSSI Collector
  { "Receive", "Left Count", "Right Count", "Left Ticks",
    "Right Ticks" },                              // Encoder 1
  { "Receive" },                                  // Encoder 2
  { "Receive" },                                  // Rover Pose
  { "Receive", "PID Compensation Recalculated" }, // PID Controller
  { "Receive", "Command Duty Cycle Set", "PID Duty Cycle Set" }, // Motor 1
  { "Receive" },                                                 // Motor 2
  {},                                                            // UART RX
  {},                                                            // UART TX
  {}, // Proto messages
  {}, // ErrorCheck
  {}, // Warnings
  { "X updated", "Y updated", "Yaw Updated" },
  {}
};

enum MOTOR1DIRECTION {
  MOTOR_TESTING,
  MOTOR_COMMAND_SET,
  MOTOR_PID_SET,
  MOTOR_FORWARD,
  MOTOR_BACKWARD,
  MOTOR_LEFT,
  MOTOR_RIGHT,
  MOTOR_STOP
};

void webserver_view::main(std::string url) {
  content::message c;

  std::string command_prefix("/command");
  std::string poseset_prefix("/poseset");
  std::string nav_prefix("/astar");

  if (url == "/") {

    auto number = std::to_string(aggregate_debug_info_count());
    auto rate = std::to_string(aggregate_debug_info_rate_per_minute());
    auto vec = aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list += "<tr><td>";
      c.message_list += std::to_string((uint32_t)DebugInfo_cpuTicks(&x));
      c.message_list += "<td>";
      c.message_list += std::string(task_names[DebugInfo_identifier(&x)]);
      c.message_list += "</td><td>";
      c.message_list += std::string(
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)]);
      c.message_list += "</td><td>";
      c.message_list += std::to_string(DebugInfo_data(&x));
      c.message_list += "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/sensors") {
    auto number = std::to_string(sensors_aggregate_debug_info_count());
    auto rate = std::to_string(sensors_debug_info_rate_per_minute());
    auto vec = sensors_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_identifier(&x)] + "</td><td>" +
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
          "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/rssi") {
    auto number = std::to_string(rssi_aggregate_debug_info_count());
    auto rate = std::to_string(rssi_debug_info_rate_per_minute());
    auto vec = rssi_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_identifier(&x)] + "</td><td>" +
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
          "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/pid") {
    auto number = std::to_string(pid_aggregate_debug_info_count());
    auto rate = std::to_string(pid_debug_info_rate_per_minute());
    auto vec = pid_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_identifier(&x)] + "</td><td>" +
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
          "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/motors") {
    auto number = std::to_string(motor_aggregate_debug_info_count());
    auto rate = std::to_string(motor_debug_info_rate_per_minute());
    auto vec = motor_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_identifier(&x)] + "</td><td>" +
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
          "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/controls") {
    render("rover_control_view", c);
  } else if (url == "/forward") {
    struct UART_TRANSMITTER_VARIANT var;
    var.type = MOTOR_MESSAGE_TX;
    MotorCommand_init(&var.data.motorMessage);
    MotorCommand_set_direction(&var.data.motorMessage, MOTOR_FORWARD);
    MotorCommand_set_dutyCycle(&var.data.motorMessage, 50);
    MotorCommand_set_mode(&var.data.motorMessage, MOTOR_COMMAND_SET);
    MotorCommand_to_bytes(&var.data.motorMessage,
                          (char *)&var.data.motorMessage, 0);
    sendToUartQueue(&var);
    render("rover_control_view", c);
  } else if (url == "/left") {
    struct UART_TRANSMITTER_VARIANT var;
    var.type = MOTOR_MESSAGE_TX;
    MotorCommand_init(&var.data.motorMessage);
    MotorCommand_set_direction(&var.data.motorMessage, MOTOR_LEFT);
    MotorCommand_set_dutyCycle(&var.data.motorMessage, 50);
    MotorCommand_set_mode(&var.data.motorMessage, MOTOR_COMMAND_SET);
    MotorCommand_to_bytes(&var.data.motorMessage,
                          (char *)&var.data.motorMessage, 0);
    sendToUartQueue(&var);
    render("rover_control_view", c);
  } else if (url == "/right") {
    struct UART_TRANSMITTER_VARIANT var;
    var.type = MOTOR_MESSAGE_TX;
    MotorCommand_init(&var.data.motorMessage);
    MotorCommand_set_direction(&var.data.motorMessage, MOTOR_RIGHT);
    MotorCommand_set_dutyCycle(&var.data.motorMessage, 50);
    MotorCommand_set_mode(&var.data.motorMessage, MOTOR_COMMAND_SET);
    MotorCommand_to_bytes(&var.data.motorMessage,
                          (char *)&var.data.motorMessage, 0);
    sendToUartQueue(&var);
    render("rover_control_view", c);
  } else if (url == "/backward") {
    struct UART_TRANSMITTER_VARIANT var;
    var.type = MOTOR_MESSAGE_TX;
    MotorCommand_init(&var.data.motorMessage);
    MotorCommand_set_direction(&var.data.motorMessage, MOTOR_BACKWARD);
    MotorCommand_set_dutyCycle(&var.data.motorMessage, 50);
    MotorCommand_set_mode(&var.data.motorMessage, MOTOR_COMMAND_SET);
    MotorCommand_to_bytes(&var.data.motorMessage,
                          (char *)&var.data.motorMessage, 0);
    sendToUartQueue(&var);
    render("rover_control_view", c);
  } else if (url == "/stop") {
    struct UART_TRANSMITTER_VARIANT var;
    var.type = MOTOR_MESSAGE_TX;
    MotorCommand_init(&var.data.motorMessage);
    MotorCommand_set_direction(&var.data.motorMessage, MOTOR_BACKWARD);
    MotorCommand_set_dutyCycle(&var.data.motorMessage, 0);
    MotorCommand_set_mode(&var.data.motorMessage, MOTOR_COMMAND_SET);
    MotorCommand_to_bytes(&var.data.motorMessage,
                          (char *)&var.data.motorMessage, 0);
    sendToUartQueue(&var);
    render("rover_control_view", c);
  } else if (url == "/errorcheck") {
    auto number = std::to_string(errorcheck_aggregate_debug_info_count());
    auto rate = std::to_string(errorcheck_debug_info_rate_per_minute());
    auto vec = errorcheck_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list += "<thead><tr><th>Timestamp "
                      "(cycles)</th><th>Task</th><th>Line "
                      "Number</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_debugID(&x)] + "</td><td>" +
          std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/warnings") {
    auto number = std::to_string(warning_aggregate_debug_info_count());
    auto rate = std::to_string(warning_debug_info_rate_per_minute());
    auto vec = warning_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list += "<thead><tr><th>Timestamp "
                      "(cycles)</th><th>Task</th><th>Line "
                      "Number</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_debugID(&x)] + "</td><td>" +
          std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/encoders") {
    auto number = std::to_string(encoders_aggregate_debug_info_count());
    auto rate = std::to_string(encoders_debug_info_rate_per_minute());
    auto vec = encoders_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_identifier(&x)] + "</td><td>" +
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
          "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (url == "/pose") {
    auto number = std::to_string(pose_aggregate_debug_info_count());
    auto rate = std::to_string(pose_debug_info_rate_per_minute());
    auto vec = pose_aggregate_info_vector();

    c.num_messages = number;
    c.data_rate = rate;
    c.message_list =
        "<div class = \"container\"><table class = \"table table-striped\">";
    c.message_list +=
        "<thead><tr><th>Timestamp (cycles)</th><th>Task</th><th>Event "
        "ID</th><th>Data</th></tr></thead><tbody>";
    for (auto itr = vec.rbegin(); itr != vec.rend(); itr++) {
      auto &x = *itr;
      c.message_list +=
          "<tr><td>" + std::to_string((uint32_t)DebugInfo_cpuTicks(&x)) +
          "<td>" + task_names[DebugInfo_identifier(&x)] + "</td><td>" +
          event_names[DebugInfo_identifier(&x)][DebugInfo_debugID(&x)] +
          "</td><td>" + std::to_string(DebugInfo_data(&x)) + "</td></tr>";
    }
    c.message_list += "</tbody></table></div>";

    render("message", c);
  } else if (std::equal(begin(command_prefix), end(command_prefix),
                        url.begin())) {
    std::regex r(R"(-?\d+)");

    auto str = request().query_string();

    auto b = std::sregex_iterator(str.begin(), str.end(), r);
    auto e = std::sregex_iterator();

    int x = 0, y = 0;
    size_t idx = 0;
    auto i = b;
    for (; i != e; i++, idx++) {
      switch (idx) {
      case 0:
        x = std::stoi(i->str());
        break;
      case 1:
        y = std::stoi(i->str());
        break;
      }
    }

    if (idx >= 2) {
      struct UART_TRANSMITTER_VARIANT var;
      var.type = CONTROL_CMD;
      RoverPose_init(&var.data.controlCmd);
      RoverPose_set_xPosition(&var.data.controlCmd, x);
      RoverPose_set_yPosition(&var.data.controlCmd, y);
      RoverPose_set_yaw(&var.data.controlCmd, 0);
      RoverPose_to_bytes(&var.data.controlCmd, (char *)&var.data.controlCmd, 0);
      sendToUartQueue(&var);
    }

    render("rover_command_view", c);
  } else if (std::equal(begin(poseset_prefix), end(poseset_prefix),
                        url.begin())) {
    std::regex r(R"(-?\d+)");

    auto str = request().query_string();

    auto b = std::sregex_iterator(str.begin(), str.end(), r);
    auto e = std::sregex_iterator();

    int x = 0, y = 0, yaw = 0;
    size_t idx = 0;
    auto i = b;
    for (; i != e; i++, idx++) {
      switch (idx) {
      case 0:
        x = std::stoi(i->str());
        break;
      case 1:
        y = std::stoi(i->str());
        break;
      case 2:
        yaw = std::stoi(i->str());
        break;
      }
    }

    if (idx >= 3) {
      struct UART_TRANSMITTER_VARIANT var;
      var.type = POSE_OVERRIDE;
      RoverPose_init(&var.data.poseOverride);
      RoverPose_set_xPosition(&var.data.poseOverride, x);
      RoverPose_set_yPosition(&var.data.poseOverride, y);
      RoverPose_set_yaw(&var.data.poseOverride, yaw);
      RoverPose_to_bytes(&var.data.poseOverride, (char *)&var.data.poseOverride,
                         0);
      sendToUartQueue(&var);
    }

    render("rover_command_view", c);
  } else if (std::equal(begin(nav_prefix), end(nav_prefix), url.begin())) {
    std::regex r(R"(-?\d+)");

    auto str = request().query_string();

    auto b = std::sregex_iterator(str.begin(), str.end(), r);
    auto e = std::sregex_iterator();

    int x = 0, y = 0;
    size_t idx = 0;
    auto i = b;
    for (; i != e; i++, idx++) {
      switch (idx) {
      case 0:
        x = std::stoi(i->str());
        break;
      case 1:
        y = std::stoi(i->str());
        break;
      }
    }

    if (idx >= 2) {
      AStarVariant var;
      var.type = ASTAR_SETPOINT;
      RoverPose_init(&var.data.astar_setpoint);
      RoverPose_set_xPosition(&var.data.astar_setpoint, x);
      RoverPose_set_yPosition(&var.data.astar_setpoint, y);
      RoverPose_to_bytes(&var.data.astar_setpoint,
                         (char *)&var.data.astar_setpoint, 0);
      sendToAStarModelQueue(&var);
    }

    content::astar_message m;
    render("astar_view", m);
  }
}
