#pragma once

namespace yk {
	//����ʹ�ñ�ͨ����2���ֱ����в��Ե�
	class Joystick {
	public:
		enum class EKey {
			kA = 0,
			kB = 1,
			kX = 2,
			kY = 3,
			kLB = 4,
			kRB = 5,
			kBack = 6,
			kStart = 7,
			kLHandle = 8, //���ҡ�����²Ŵ���
			kRHandle = 9, //�Ҳ�ҡ�����²Ŵ���
			kDirectionUp = 10,
			kDirectionRight = 11,
			kkDirectionDown = 12,
			kDirectionLeft = 13,
		};
	public:
		Joystick();
		virtual ~Joystick();

	};

}