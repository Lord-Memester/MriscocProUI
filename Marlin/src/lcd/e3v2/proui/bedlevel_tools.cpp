/**
 * Bed Level Tools for Pro UI
 * Extended by: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.2.0
 * Date: 2023/05/03
 *
 * Based on the original work of: Henri-J-Norden
 * https://github.com/Jyers/Marlin/pull/126
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_LEVELING)

#include "../../marlinui.h"
#include "../../../core/types.h"
#include "../../../feature/bedlevel/bedlevel.h"
#include "../../../module/probe.h"
#include "../../../gcode/gcode.h"
#include "../../../module/planner.h"
#include "../../../gcode/queue.h"
#include "../../../libs/least_squares_fit.h"
#include "../../../libs/vector_3.h"

#include "dwin.h"
#include "dwinui.h"
#include "dwin_popup.h"
#include "bedlevel_tools.h"

BedLevelToolsClass bedLevelTools;

#if ENABLED(USE_GRID_MESHVIEWER)
  bool BedLevelToolsClass::viewer_asymmetric_range = false;
  bool BedLevelToolsClass::viewer_print_value = true;
  bool BedLevelToolsClass::view_mesh = false;
#endif
bool BedLevelToolsClass::goto_mesh_value = false;
uint8_t BedLevelToolsClass::mesh_x = 0;
uint8_t BedLevelToolsClass::mesh_y = 0;
uint8_t BedLevelToolsClass::tilt_grid = 2;

bool drawing_mesh = false;
char str_1[16], str_2[16], str_3[16];

#if ENABLED(AUTO_BED_LEVELING_UBL)

  void BedLevelToolsClass::manual_value_update(const uint8_t mesh_x, const uint8_t mesh_y, bool undefined/*=false*/) {
    MString<MAX_CMD_SIZE> cmd;
    cmd.set(F("M421 I"), mesh_x, 'J', mesh_y, 'Z', p_float_t(current_position.z, 3));
    if (undefined) cmd += F(" N");
    gcode.process_subcommands_now(cmd);
    planner.synchronize();
  }

  bool BedLevelToolsClass::create_plane_from_mesh() {
    struct linear_fit_data lsf_results;
    incremental_LSF_reset(&lsf_results);
    GRID_LOOP(x, y) {
      if (!isnan(bedlevel.z_values[x][y])) {
        xy_pos_t rpos = { bedlevel.get_mesh_x(x), bedlevel.get_mesh_y(y) };
        incremental_LSF(&lsf_results, rpos, bedlevel.z_values[x][y]);
      }
    }

    if (finish_incremental_LSF(&lsf_results)) {
      SERIAL_ECHOPGM("Could not complete LSF!");
      return true;
    }

    bedlevel.set_all_mesh_points_to_value(0);

    matrix_3x3 rotation = matrix_3x3::create_look_at(vector_3(lsf_results.A, lsf_results.B, 1));
    GRID_LOOP(i, j) {
      float mx = bedlevel.get_mesh_x(i), my = bedlevel.get_mesh_y(j), mz = bedlevel.z_values[i][j];

      if (DEBUGGING(LEVELING)) {
        DEBUG_ECHOLN(F("before rotation = ["), p_float_t(mx, 7), AS_CHAR(','), p_float_t(my, 7), AS_CHAR(','), p_float_t(mz, 7), F("]   ---> "));
        DEBUG_DELAY(20);
      }

      rotation.apply_rotation_xyz(mx, my, mz);

      if (DEBUGGING(LEVELING)) {
        DEBUG_ECHOLN(F("after rotation = ["), p_float_t(mx, 7), AS_CHAR(','), p_float_t(my, 7), AS_CHAR(','), p_float_t(mz, 7), F("]   ---> "));
        DEBUG_DELAY(20);
      }

      bedlevel.z_values[i][j] = mz - lsf_results.D;
    }
    return false;
  }

#else

  void BedLevelToolsClass::manual_value_update(const uint8_t mesh_x, const uint8_t mesh_y) {
    gcode.process_subcommands_now(
      TS(F("G29 I"), mesh_x, 'J', mesh_y, 'Z', p_float_t(current_position.z, 3))
    );
    planner.synchronize();
  }

#endif

void BedLevelToolsClass::manual_move(const uint8_t mesh_x, const uint8_t mesh_y, bool zmove/*=false*/) {
  gcode.process_subcommands_now(F("G28O"));
  if (zmove) {
    planner.synchronize();
    current_position.z = goto_mesh_value ? bedlevel.z_values[mesh_x][mesh_y] : Z_CLEARANCE_BETWEEN_PROBES;
    planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
    planner.synchronize();
  }
  else {
    DWIN_Show_Popup(ICON_BLTouch, F("Moving to Point"), F("Please wait until done."));
    HMI_SaveProcessID(NothingToDo);
    gcode.process_subcommands_now(TS(F("G0 F300 Z"), p_float_t(Z_CLEARANCE_BETWEEN_PROBES, 3)));
    gcode.process_subcommands_now(TS(F("G42 F4000 I"), mesh_x, F(" J"), mesh_y));
    planner.synchronize();
    current_position.z = goto_mesh_value ? bedlevel.z_values[mesh_x][mesh_y] : Z_CLEARANCE_BETWEEN_PROBES;
    planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
    planner.synchronize();
    HMI_ReturnScreen();
  }
}

// Move / Probe methods. As examples, not yet used.
void BedLevelToolsClass::MoveToXYZ() {
  bedLevelTools.goto_mesh_value = true;
  bedLevelTools.manual_move(bedLevelTools.mesh_x, bedLevelTools.mesh_y, false);
}
void BedLevelToolsClass::MoveToXY() {
  bedLevelTools.goto_mesh_value = false;
  bedLevelTools.manual_move(bedLevelTools.mesh_x, bedLevelTools.mesh_y, false);
}
void BedLevelToolsClass::MoveToZ() {
  bedLevelTools.goto_mesh_value = true;
  bedLevelTools.manual_move(bedLevelTools.mesh_x, bedLevelTools.mesh_y, true);
}
void BedLevelToolsClass::ProbeXY() {
  gcode.process_subcommands_now(
    MString<MAX_CMD_SIZE>(
      F("G28O\nG0Z"), uint16_t(Z_CLEARANCE_DEPLOY_PROBE),
      F("\nG30X"), p_float_t(bedlevel.get_mesh_x(bedLevelTools.mesh_x), 2),
      F("Y"), p_float_t(bedlevel.get_mesh_y(bedLevelTools.mesh_y), 2)
    )
  );
}

void BedLevelToolsClass::mesh_reset() {
  ZERO(bedlevel.z_values);
  TERN_(AUTO_BED_LEVELING_BILINEAR, bedlevel.refresh_bed_level();)
}

// Accessors
float BedLevelToolsClass::get_max_value() {
  float max = __FLT_MAX__ * -1;
  GRID_LOOP(x, y) {
    if (!isnan(bedlevel.z_values[x][y]) && bedlevel.z_values[x][y] > max)
      max = bedlevel.z_values[x][y];
  }
  return max;
}

float BedLevelToolsClass::get_min_value() {
  float min = __FLT_MAX__;
  GRID_LOOP(x, y) {
    if (!isnan(bedlevel.z_values[x][y]) && bedlevel.z_values[x][y] < min)
      min = bedlevel.z_values[x][y];
  }
  return min;
}

// Return 'true' if mesh is good and within LCD limits
bool BedLevelToolsClass::meshValidate() {
  GRID_LOOP(x, y) {
    const float v = bedlevel.z_values[x][y];
    if (isnan(v) || !WITHIN(v, UBL_Z_OFFSET_MIN, UBL_Z_OFFSET_MAX)) return false;
  }
  return true;
}

#if ENABLED(USE_GRID_MESHVIEWER)

  constexpr uint8_t meshfont = TERN(TJC_DISPLAY, font8x16, font6x12);

  void BedLevelToolsClass::Draw_Bed_Mesh(int16_t selected/*=-1*/, uint8_t gridline_width/*=1*/, uint16_t padding_x/*=8*/, uint16_t padding_y_top/*=(40 + 53 - 7)*/) {
    drawing_mesh = true;
    const uint16_t total_width_px = DWIN_WIDTH - padding_x - padding_x;
    const uint16_t cell_width_px  = total_width_px / (GRID_MAX_POINTS_X);
    const uint16_t cell_height_px = total_width_px / (GRID_MAX_POINTS_Y);
    const float v_max = abs(get_max_value()), v_min = abs(get_min_value()), range = _MAX(v_min, v_max);

    // Clear background from previous selection and select new square
    DWIN_Draw_Rectangle(1, Color_Bg_Black, _MAX(0, padding_x - gridline_width), _MAX(0, padding_y_top - gridline_width), padding_x + total_width_px, padding_y_top + total_width_px);
    if (selected >= 0) {
      const auto selected_y = selected / (GRID_MAX_POINTS_X);
      const auto selected_x = selected - (GRID_MAX_POINTS_X) * selected_y;
      const auto start_y_px = padding_y_top + selected_y * cell_height_px;
      const auto start_x_px = padding_x + selected_x * cell_width_px;
      DWIN_Draw_Rectangle(1, Color_White, _MAX(0, start_x_px - gridline_width), _MAX(0, start_y_px - gridline_width), start_x_px + cell_width_px, start_y_px + cell_height_px);
    }

    // Draw value square grid
    GRID_LOOP(x, y) {
      const auto start_x_px = padding_x + x * cell_width_px;
      const auto end_x_px   = start_x_px + cell_width_px - 1 - gridline_width;
      const auto start_y_px = padding_y_top + ((GRID_MAX_POINTS_Y) - y - 1) * cell_height_px;
      const auto end_y_px   = start_y_px + cell_height_px - 1 - gridline_width;
      DWIN_Draw_Rectangle(1,                                                                                 // RGB565 colors: http://www.barth-dev.de/online/rgb565-color-picker/
        isnan(bedlevel.z_values[x][y]) ? Color_Grey : (                                                           // gray if undefined
          (bedlevel.z_values[x][y] < 0 ?
            (uint16_t)round(0x1F * -bedlevel.z_values[x][y] / (!viewer_asymmetric_range ? range : v_min)) << 11 : // red if mesh point value is negative
            (uint16_t)round(0x3F *  bedlevel.z_values[x][y] / (!viewer_asymmetric_range ? range : v_max)) << 5) | // green if mesh point value is positive
              _MIN(0x1F, (((uint8_t)abs(bedlevel.z_values[x][y]) / 10) * 4))),                                    // + blue stepping for every mm
        start_x_px, start_y_px, end_x_px, end_y_px
      );

      safe_delay(10);
      LCD_SERIAL.flushTX();

      // Draw value text on
      char buf[8];
      const uint8_t fs = DWINUI::fontWidth(meshfont);
      if (viewer_print_value) {
        int8_t offset_x, offset_y = cell_height_px / 2 - fs;
        if (isnan(bedlevel.z_values[x][y])) {  // undefined
          DWIN_Draw_String(false, meshfont, Color_White, Color_Bg_Blue, start_x_px + cell_width_px / 2 - 5, start_y_px + offset_y, F("X"));
        }
        else {                          // has value
          if (GRID_MAX_POINTS_X < (ENABLED(TJC_DISPLAY) ? 8 : 10))
            sprintf_P(buf, PSTR("%s"), dtostrf(abs(bedlevel.z_values[x][y]), 1, 2, str_1));
          else
            sprintf_P(buf, PSTR("%02i"), (uint16_t)(abs(bedlevel.z_values[x][y] - (int16_t)bedlevel.z_values[x][y]) * 100));
          offset_x = cell_width_px / 2 - (fs/2) * (strlen(buf)) - 2;
          if (!(GRID_MAX_POINTS_X < (ENABLED(TJC_DISPLAY) ? 8 : 10)))
            DWIN_Draw_String(false, meshfont, Color_White, Color_Bg_Blue, start_x_px - 2 + offset_x, start_y_px + offset_y, F("."));
          DWIN_Draw_String(false, meshfont, Color_White, Color_Bg_Blue, start_x_px + 1 + offset_x, start_y_px + offset_y, buf);
        }
        safe_delay(10);
        LCD_SERIAL.flushTX();
      }
    }
  }

  void BedLevelToolsClass::Set_Mesh_Viewer_Status() { // TODO: draw gradient with values as a legend instead
    float v_max = abs(get_max_value()), v_min = abs(get_min_value()), rangeA = _MAX(v_min, v_max), rangeB = _MIN(v_min, v_max);
    if (v_min > 3e+10F) { v_min = 0.0000001; }
    if (v_max > 3e+10F) { v_max = 0.0000001; }
    if (rangeA > 3e+10F) { rangeA = 0.0000001; }
    if (rangeB > 3e+10F) { rangeB = 0.0000001; }
    ui.set_status(
      &MString<47>(
        F("Red "),  p_float_t(viewer_asymmetric_range ? -v_min : -rangeA, 3),
        F("..0.."), p_float_t(viewer_asymmetric_range ?  v_max :  rangeB, 3),
        F("+ Green")
      )
    );
    drawing_mesh = false;
  }

#endif // USE_GRID_MESHVIEWER

#endif // DWIN_LCD_PROUI && HAS_LEVELING
