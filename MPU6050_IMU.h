#ifndef MPU6050_IMU_H
#define MPU6050_IMU_H

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR 0x68

#define SDA_PIN 21
#define SCL_PIN 22

class MPU6050_IMU {

private:
    // Raw LSB
    int16_t ax_raw, ay_raw, az_raw;
    int16_t gx_raw, gy_raw, gz_raw;

    // Converted values
    float ax, ay, az;
    float gx, gy, gz;

    // Angles
    float roll, pitch;

    // Bias
    float gx_bias = 0;
    float gy_bias = 0;
    float gz_bias = 0;

public:

    struct IMUData {
        float AccX;
        float AccY;
        float AccZ;
        float GyroX;
        float GyroY;
        float GyroZ;
        float Roll;
        float Pitch;
        float AccMagnitude;
        float GyroMagnitude;
    };

    /* ---------------- INIT ---------------- */
    void begin(uint8_t sda = SDA_PIN, uint8_t scl = SCL_PIN) {

        Wire.begin(sda, scl, 400000);

        // Wake up MPU
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x6B);
        Wire.write(0x00);
        Wire.endTransmission();

        // Low pass filter
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x1A);
        Wire.write(0x05);
        Wire.endTransmission();

        // Gyro ±500 deg/s
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x1B);
        Wire.write(0x08);
        Wire.endTransmission();

        // Accel ±8g
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x1C);
        Wire.write(0x10);
        Wire.endTransmission();
    }

    /* ---------------- READ SENSOR ---------------- */
    void update() {

        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x3B);
        Wire.endTransmission();
        Wire.requestFrom(MPU_ADDR, 14);

        ax_raw = Wire.read() << 8 | Wire.read();
        ay_raw = Wire.read() << 8 | Wire.read();
        az_raw = Wire.read() << 8 | Wire.read();

        Wire.read(); Wire.read(); // skip temp

        gx_raw = Wire.read() << 8 | Wire.read();
        gy_raw = Wire.read() << 8 | Wire.read();
        gz_raw = Wire.read() << 8 | Wire.read();

        // Convert
        ax = ax_raw / 4096.0;
        ay = ay_raw / 4096.0;
        az = az_raw / 4096.0;

        gx = (gx_raw / 65.5) - gx_bias;
        gy = (gy_raw / 65.5) - gy_bias;
        gz = (gz_raw / 65.5) - gz_bias;

        // Angle calculation
        roll  = atan2(ay, sqrt(ax * ax + az * az)) * 180 / PI;
        pitch = -atan2(ax, sqrt(ay * ay + az * az)) * 180 / PI;
    }

    /* ---------------- CALIBRATION ---------------- */
    void calibrateGyro(int samples = 2000) {

        float sx = 0, sy = 0, sz = 0;

        for(int i = 0; i < samples; i++) {
            update();
            sx += gx_raw / 65.5;
            sy += gy_raw / 65.5;
            sz += gz_raw / 65.5;
            delay(2);
        }

        gx_bias = sx / samples;
        gy_bias = sy / samples;
        gz_bias = sz / samples;
    }

    /* ---------------- GET INDIVIDUAL DATA ---------------- */

    float getAccX() { return ax; }
    float getAccY() { return ay; }
    float getAccZ() { return az; }

    float getGyroX() { return gx; }
    float getGyroY() { return gy; }
    float getGyroZ() { return gz; }

    float getRoll()  { return roll; }
    float getPitch() { return pitch; }

    float getAccMagnitude() {
        return sqrt(ax*ax + ay*ay + az*az);
    }

    float getGyroMagnitude() {
        return sqrt(gx*gx + gy*gy + gz*gz);
    }

    /* ---------------- GET COMPLETE DATA ---------------- */

    IMUData getAllData() {

        IMUData data;

        data.AccX = ax;
        data.AccY = ay;
        data.AccZ = az;

        data.GyroX = gx;
        data.GyroY = gy;
        data.GyroZ = gz;

        data.Roll  = roll;
        data.Pitch = pitch;

        data.AccMagnitude  = getAccMagnitude();
        data.GyroMagnitude = getGyroMagnitude();

        return data;
    }
};

#endif
