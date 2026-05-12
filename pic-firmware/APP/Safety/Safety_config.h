/*
 * Safety_config.h
 * Irrigation Project — Safety Feature Thresholds
 *
 * NOTE: Dry-Run protection (ultrasonic) removed.
 */

#ifndef SAFETY_CONFIG_H
#define SAFETY_CONFIG_H

/* ------------------------------------------------------
   4.2 OVERWATERING PROTECTION (Soil Moisture)
   ------------------------------------------------------ */
#define SAFETY_SOIL_WARN_PCT        80u   /* Level 1 Warning  (>80%)  */
#define SAFETY_SOIL_CRITICAL_PCT    95u   /* Level 2 Action   (>95%)  */
#define SAFETY_SOIL_RECOVERY_PCT    65u   /* Recovery: irrigation re-enable */

/* ------------------------------------------------------
   4.3 OVERCURRENT PROTECTION (ACS712)
   ------------------------------------------------------ */
#define SAFETY_CURRENT_RATED_MA     500u   /* Nominal pump current (mA)   */
#define SAFETY_CURRENT_WARN_MA      600u   /* 120% rated = Warning         */
#define SAFETY_CURRENT_CRITICAL_MA  750u   /* 150% rated = Full shutdown   */
#define SAFETY_CURRENT_PERSIST_CNT    5u   /* Consecutive high readings    */

/* ------------------------------------------------------
   4.4 OVERHEAT PROTECTION (LM35 Temperature)
   ------------------------------------------------------ */
#define SAFETY_TEMP_WARN_C          45     /* Level 1 Warning (°C)         */
#define SAFETY_TEMP_CRITICAL_C      55     /* Level 2 Action  (°C)         */
#define SAFETY_TEMP_RECOVERY_C      40     /* Recovery: 15°C hysteresis    */

#endif /* SAFETY_CONFIG_H */