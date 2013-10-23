#include "SysInfo_in.h"
#ifdef PLATFORM_POSIX

NAMESPACE_UPP

#include <sensors/sensors.h>
#include <sensors/error.h>

String GetSensorsVersion() {
    return libsensors_version;
}

static const char *GetFeatureTypeText(sensors_feature_type type) {
	switch(type) {
	case SENSORS_FEATURE_IN:			return "in";
	case SENSORS_FEATURE_FAN:			return "fan";
	case SENSORS_FEATURE_TEMP:			return "temperature";
	case SENSORS_FEATURE_POWER:			return "power";
	case SENSORS_FEATURE_ENERGY:		return "energy";
	case SENSORS_FEATURE_CURR:			return "current";
	case SENSORS_FEATURE_HUMIDITY:		return "humidity";
	case SENSORS_FEATURE_MAX_MAIN:		return "max main";
	case SENSORS_FEATURE_VID:			return "video";
	case SENSORS_FEATURE_INTRUSION:		return "intrusion";
	case SENSORS_FEATURE_MAX_OTHER:		return "max other";
	case SENSORS_FEATURE_BEEP_ENABLE:	return "beep enable";
	}
	return "unknown";	
}

static const char *GetSubFeatureTypeText(sensors_subfeature_type type) {
	switch(type) {
	case SENSORS_SUBFEATURE_IN_INPUT:				return "input";
	case SENSORS_SUBFEATURE_IN_MIN:					return "min";
	case SENSORS_SUBFEATURE_IN_MAX:					return "max";
	case SENSORS_SUBFEATURE_IN_LCRIT:				return "lcrit";
	case SENSORS_SUBFEATURE_IN_CRIT:				return "crit";
	case SENSORS_SUBFEATURE_IN_AVERAGE:				return "average";
	case SENSORS_SUBFEATURE_IN_LOWEST:				return "lowest";
	case SENSORS_SUBFEATURE_IN_HIGHEST:				return "highest";
	case SENSORS_SUBFEATURE_IN_ALARM:				return "alarm";
	case SENSORS_SUBFEATURE_IN_MIN_ALARM:			return "min alarm";
	case SENSORS_SUBFEATURE_IN_MAX_ALARM:			return "max alarm";
	case SENSORS_SUBFEATURE_IN_BEEP:				return "beep";
	case SENSORS_SUBFEATURE_IN_LCRIT_ALARM:			return "lcrit alarm";
	case SENSORS_SUBFEATURE_IN_CRIT_ALARM:			return "crit alarm";

	case SENSORS_SUBFEATURE_FAN_INPUT:				return "input";
	case SENSORS_SUBFEATURE_FAN_MIN:				return "min";
	case SENSORS_SUBFEATURE_FAN_MAX:				return "max";
	case SENSORS_SUBFEATURE_FAN_ALARM:				return "alarm";
	case SENSORS_SUBFEATURE_FAN_FAULT:				return "fault";
	case SENSORS_SUBFEATURE_FAN_DIV:				return "div";
	case SENSORS_SUBFEATURE_FAN_BEEP:				return "beep";
	case SENSORS_SUBFEATURE_FAN_PULSES:				return "pulses";
	case SENSORS_SUBFEATURE_FAN_MIN_ALARM:			return "min alarm";
	case SENSORS_SUBFEATURE_FAN_MAX_ALARM:			return "max alarm";

	case SENSORS_SUBFEATURE_TEMP_INPUT:				return "input";
	case SENSORS_SUBFEATURE_TEMP_MAX:				return "max";
	case SENSORS_SUBFEATURE_TEMP_MAX_HYST:			return "max hysteresis";
	case SENSORS_SUBFEATURE_TEMP_MIN:				return "min";
	case SENSORS_SUBFEATURE_TEMP_CRIT:				return "critical";
	case SENSORS_SUBFEATURE_TEMP_CRIT_HYST:			return "crit hysteresis";
	case SENSORS_SUBFEATURE_TEMP_LCRIT:				return "lcrit";
	case SENSORS_SUBFEATURE_TEMP_EMERGENCY:			return "emergency";
	case SENSORS_SUBFEATURE_TEMP_EMERGENCY_HYST:	return "emergency hysteresis";
	case SENSORS_SUBFEATURE_TEMP_LOWEST:			return "lowest";
	case SENSORS_SUBFEATURE_TEMP_HIGHEST:			return "highest";
	case SENSORS_SUBFEATURE_TEMP_ALARM:				return "alarm";
	case SENSORS_SUBFEATURE_TEMP_MAX_ALARM:			return "max alarm";
	case SENSORS_SUBFEATURE_TEMP_MIN_ALARM:			return "min alarm";
	case SENSORS_SUBFEATURE_TEMP_CRIT_ALARM:		return "crit alarm";
	case SENSORS_SUBFEATURE_TEMP_FAULT:				return "fault";
	case SENSORS_SUBFEATURE_TEMP_TYPE:				return "type";
	case SENSORS_SUBFEATURE_TEMP_OFFSET:			return "offset";
	case SENSORS_SUBFEATURE_TEMP_BEEP:				return "beep";
	case SENSORS_SUBFEATURE_TEMP_EMERGENCY_ALARM:	return "emergency alarm";
	case SENSORS_SUBFEATURE_TEMP_LCRIT_ALARM:		return "lcrit alarm";

	case SENSORS_SUBFEATURE_POWER_AVERAGE:			return "average";
	case SENSORS_SUBFEATURE_POWER_AVERAGE_HIGHEST:	return "average highest";
	case SENSORS_SUBFEATURE_POWER_AVERAGE_LOWEST:	return "average lowest";
	case SENSORS_SUBFEATURE_POWER_INPUT:			return "input";
	case SENSORS_SUBFEATURE_POWER_INPUT_HIGHEST:	return "highest";
	case SENSORS_SUBFEATURE_POWER_INPUT_LOWEST:		return "lowest";
	case SENSORS_SUBFEATURE_POWER_CAP:				return "cap";
	case SENSORS_SUBFEATURE_POWER_CAP_HYST:			return "cap hysteresis";
	case SENSORS_SUBFEATURE_POWER_MAX:				return "max";
	case SENSORS_SUBFEATURE_POWER_CRIT:				return "critical";
	case SENSORS_SUBFEATURE_POWER_AVERAGE_INTERVAL:	return "average interval";
	case SENSORS_SUBFEATURE_POWER_ALARM:			return "alarm";
	case SENSORS_SUBFEATURE_POWER_CAP_ALARM:		return "cap alarm";
	case SENSORS_SUBFEATURE_POWER_MAX_ALARM:		return "max alarm";
	case SENSORS_SUBFEATURE_POWER_CRIT_ALARM:		return "critical alarm";

	case SENSORS_SUBFEATURE_ENERGY_INPUT:			return "input";

	case SENSORS_SUBFEATURE_CURR_INPUT:				return "input";
	case SENSORS_SUBFEATURE_CURR_MIN:				return "min";
	case SENSORS_SUBFEATURE_CURR_MAX:				return "max";
	case SENSORS_SUBFEATURE_CURR_LCRIT:				return "lcrit";
	case SENSORS_SUBFEATURE_CURR_CRIT:				return "critical";
	case SENSORS_SUBFEATURE_CURR_AVERAGE:			return "average";
	case SENSORS_SUBFEATURE_CURR_LOWEST:			return "lowest";
	case SENSORS_SUBFEATURE_CURR_HIGHEST:			return "highest";
	case SENSORS_SUBFEATURE_CURR_ALARM:				return "alarm";
	case SENSORS_SUBFEATURE_CURR_MIN_ALARM:			return "min alarm";
	case SENSORS_SUBFEATURE_CURR_MAX_ALARM:			return "max alarm";
	case SENSORS_SUBFEATURE_CURR_BEEP:				return "beep";
	case SENSORS_SUBFEATURE_CURR_LCRIT_ALARM:		return "lcrit alarm";
	case SENSORS_SUBFEATURE_CURR_CRIT_ALARM:		return "crit alarm";

	case SENSORS_SUBFEATURE_HUMIDITY_INPUT:			return "input";

	case SENSORS_SUBFEATURE_VID:					return "";

	case SENSORS_SUBFEATURE_INTRUSION_ALARM:		return "alarm";
	case SENSORS_SUBFEATURE_INTRUSION_BEEP:			return "beep";

	case SENSORS_SUBFEATURE_BEEP_ENABLE:			return "enable";
	}
	return "unknown";
}

static String FetchSensors() {
	sensors_init(NULL);
	String ret;
	
	const sensors_chip_name *chip;
	int c = 0;
    while ((chip = sensors_get_detected_chips(0, &c)) != 0) {
        Buffer<char> str(251);
        if (sensors_snprintf_chip_name(str, 250, chip) < 0)
			ret << "Chip: " << chip->prefix << "/" << chip->path << "\n";
        else
            ret << "Chip: " << String(str) << "\n";

		const sensors_feature *feature;
		int f = 0;
        while ((feature = sensors_get_features(chip, &f)) != 0) {
            ret << "\t" << GetFeatureTypeText(feature->type);
            char *label;
            if (!(label = sensors_get_label(chip, feature)))
                ret << ": " << feature->name << "\n";
            else
                ret << ": " << label << "\n";

			const sensors_subfeature *subfeat;
			int s = 0;
            while ((subfeat = sensors_get_all_subfeatures(chip, feature, &s)) != 0) {
                String type;
             	if (subfeat->flags & SENSORS_MODE_R) 	// Readable
             		type << "R";
             	if (subfeat->flags & SENSORS_MODE_W) 	// Writable
             		type << "W";
             	if (subfeat->flags & SENSORS_COMPUTE_MAPPING) 	// Affected by the computation rules of the main feature
             		type << "M";
            	ret << "\t\t" << GetSubFeatureTypeText(subfeat->type) << "(" << type << ")\t" << subfeat->name << " = ";
            	double val;
                int rc = sensors_get_value(chip, subfeat->number, &val);
                if (rc < 0) 
                    ret << "err: " << sensors_strerror(rc);
                else 
                    ret << val;
                ret << "\n";
            }
        }
    }
    sensors_cleanup();
    return ret;
}

void GetSensors() {
	Cout() << "LibSensors version: " << GetSensorsVersion() << "\n";
	Cout() << "Results:" << "\n" << FetchSensors() << "\n";	
}

END_UPP_NAMESPACE

#endif