

#ifndef RTT_ACK_HPP
#define RTT_ACK_HPP

#include <uORB/topics/rtt_ack.h>
#include <px4_platform_common/log.h>

class MavlinkStreamRTTAck : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamRTTAck(mavlink); }

	static constexpr const char *get_name_static() { return "RTT_ACK"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_RTT_ACK; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return _rtt_ack_sub.advertised() ? (MAVLINK_MSG_ID_RTT_ACK_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES) : 0;
	}

	int cnt = 0;

private:
	explicit MavlinkStreamRTTAck(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription _rtt_ack_sub{ORB_ID(rtt_ack)};

	bool send() override
	{
		rtt_ack_s rtt_ack;

		if (_rtt_ack_sub.update(&rtt_ack)) {
			mavlink_rtt_ack_t msg{};

			msg.syn_send_time_usec = rtt_ack.syn_send_time_usec;

			mavlink_msg_rtt_ack_send_struct(_mavlink->get_channel(), &msg);

			//for debug
			cnt++;
			PX4_INFO("RTT_ACK: No. %d syn_send_time_usec: %llu", cnt, rtt_ack.syn_send_time_usec);


			return true;
		}

		return false;
	}
};

#endif // RTT_ACK_HPP
