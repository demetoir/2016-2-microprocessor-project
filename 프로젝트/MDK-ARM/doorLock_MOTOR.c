///*----------------------------------------------------------------------------
// *      doorLock_L.MOTOR
// *----------------------------------------------------------------------------
// *      Name:    doorLock_MOTOR.c
// *---------------------------------------------------------------------------*/

#ifndef doorLock_MOTOR_H
#include "doorLock_MOTOR.h"
#endif // !doorLock_MOTOR_H

//������ ���¸� �����ϴ� ���� 
int LockerState = LOCKER_CLOSE;
//������ ���°� ������ �����ϴ� �÷��� ����
bool isNeedLockerMotorMove = false;

//������ GPIO setting�� �����ϴ� ����
LPC_GPIO_TypeDef    motor_GPIO_SETTING[5];

//���͸� �ʱ�ȭ�ϴ� �Լ� 
//������ GPIO�� �����ϰ� �ʱ�ȭ �׽�Ʈ�� ������ �Ѵ�
void MOTOR_init() {
	//������ GPIO�� EXT_IO�� �����Ѵ�
	set_EXT_IO_DIRECTION('A');
	Motor_Init();

	//�ʱ�ȭ �׽�Ʈ ����
	//���͸� �ݽð�������� �ι��� ȸ����Ų��
	MOTOR_moveCCWHalfCycle(); 
	Delay(MOTOR_DELAY_TIME);
	MOTOR_moveCCWHalfCycle();
	Delay(MOTOR_DELAY_TIME);
	MOTOR_moveCCWHalfCycle();
	Delay(MOTOR_DELAY_TIME);
	MOTOR_moveCCWHalfCycle();
	Delay(MOTOR_DELAY_TIME);

	//������ GPIO�� �����Ѵ�
	save_LCP_GPIO_settingTo(motor_GPIO_SETTING);
}

//motor�� �ݽð�������� �ݹ��������̵��� ����� 
//���ܸ��ʹ� ���ο� 2���� ���ڼ� a,b�� ������.
//�ΰ��� ���ڼ��� ���������� �����ϴ°��� �ϳ��� �����̸�
//�̷��� ������ �������� ��ġ�°����� �ϳ��� ���� ����Ŭ��
//�����Ѵ�. ���� �̷� 12���� ���ܻ���Ŭ�� �ݺ��ϴ°�����
//���͸� �ѹ��� �����̰� �ȴ�
//���� �ݹ����� �����̷��� 12���ܻ���Ŭ�� ���� 6���� 
//���ܻ���Ŭ�� ����ȴ�.
//���� �ϳ��� ������ �ҋ����� ���Ͱ� �����̴� �ð��� �ֱ�����
//�����ð���ŭ�� �����̰� �ʿ��ϴ�
void MOTOR_moveCCWHalfCycle() {
	int count = 0;
	for (count = 0; count < 6; count++) {
		//a, b �������� ������ �기��
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		//���Ͱ� ������ �ð��� �ֱ����� ������
		Delay(MOTOR_DELAY_TIME);
		
		//na, b �������� ������ �기��
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		//���Ͱ� ������ �ð��� �ֱ����� ������
		Delay(MOTOR_DELAY_TIME);

		//na, nb �������� ������ �기��
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		//���Ͱ� ������ �ð��� �ֱ����� ������
		Delay(MOTOR_DELAY_TIME);

		//a, nb �������� ������ �기��
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		//���Ͱ� ������ �ð��� �ֱ����� ������
		Delay(MOTOR_DELAY_TIME);
	}
}

//���͸� ������ ��Ŀ�� ������ ���� ���¸� �����Ѵ�
void MOTOR_setOpenLocker() {
	//������ ��Ŀ���¿� �ٸ����� ���͸� �����̵��� �Ѵ�
	if (LockerState != LOCKER_OPEN)
		isNeedLockerMotorMove = true;
	LockerState = LOCKER_OPEN;
}

//���͸� ������ ��Ŀ�� �ݵ��� ���� ���¸� �����Ѵ�
void MOTOR_setCloseLocker() {
	//������ ��Ŀ���¿� �ٸ����� ���͸� �����̵��� �Ѵ�
	if (LockerState != LOCKER_CLOSE)
		isNeedLockerMotorMove = true;
	LockerState = LOCKER_CLOSE;
}

//������ mainTask�� �����ϴ� �Լ�
void MOTOR_mainTask() {
	//���Ͱ� ���������� ���ٸ� �ٷ� �����Ѵ�
	if (isNeedLockerMotorMove == false) 
		return;
	//�ߺ����� ���Ͱ� ������ ������ ���� �÷��� ������ false�� �����Ѵ�
	isNeedLockerMotorMove = false;

	//������ GPIO�� ���� �ε��Ѵ�
	load_LPC_GPIO_settingFrom(motor_GPIO_SETTING);

	//Locker�� ���¿����� ���͸� ������ ��Ŀ�� ���ų� �ݴ´�
	if (LockerState == LOCKER_OPEN) {
		MOTOR_moveCCWHalfCycle();
	}
	else if (LockerState == LOCKER_CLOSE) {
		MOTOR_moveCCWHalfCycle();
	}	
}
