# ifndef RTT_SYN_HPP
# define RTT_SYN_HPP

# include <uORB/topics/rtt_syn.h>

class MavlinkStreamRTTSyn : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamRTTSyn(mavlink); }

	static constexpr const char *get_name_static() { return "RTT_SYN"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_RTT_SYN; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return _rtt_syn_sub.advertised() ? (MAVLINK_MSG_ID_RTT_SYN_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES) : 0;
	}

private:
	explicit MavlinkStreamRTTSyn(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription _rtt_syn_sub{ORB_ID(rtt_syn)};

	bool send() override
	{
		rtt_syn_s rtt_syn;

		if (_rtt_syn_sub.update(&rtt_syn)) {
			mavlink_rtt_syn_t msg{};

			msg.syn_send_time_usec = rtt_syn.syn_send_time_usec;

			mavlink_msg_rtt_syn_send_struct(_mavlink->get_channel(), &msg);

			return true;
		}

		return false;
	}
};

# endif // RTT_SYN_HPP
