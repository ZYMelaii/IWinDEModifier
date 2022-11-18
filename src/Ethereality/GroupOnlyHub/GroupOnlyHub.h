#pragma once

#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include <optional>
#include <nlohmann/json.hpp>

#include "../msgitem.h"
#include "GroupMsgFactory.h"

class GroupOnlyHub : public ChatHub {
private:
	using FactoryModel = std::unique_ptr<MsgItemFactory>;
	using json		   = nlohmann::json;
	using ItemType	   = std::pair<uint64_t, json>;

	std::mutex				mutex_;
	std::queue<std::string> req_queue_;

	std::string identity_;

	mutable FactoryModel  factory_;
	bool				  should_render_;
	std::vector<ItemType> items_;
	ImVec2				  size_;

	float  scroll_state_;
	ImVec2 scroll_range_;
	int	   scroll_anime_state_;
	bool   scroll_lock_;

public:
	GroupOnlyHub();

	void					   login_as(const std::string& identity);
	const std::string&		   getIdentity() const;
	std::optional<std::string> getRequest();
	void					   addRequest(std::string req);


	void push(const std::string& sender, const std::string& msgid, const std::string& text);
	json parse(uint64_t id);

	float getScrollState() const;
	void  scrollTo(float cursor);
	void  resize(ImVec2 size);
	void  renderScrollBar(const ImRect& bb);

	void resolvePanel(const ImRect& bb);
	void resolveInput(const ImRect& bb);
	void render(ImVec2 pos, ImVec2 size);

public:
	virtual bool	 bindFactory(MsgItemFactory* factory) const override;
	virtual bool	 shouldRender() const override;
	virtual ImVec2	 getMinSizeConstraint() const override;
	virtual uint64_t queryNext(uint64_t id) override;
	virtual uint64_t queryPrevious(uint64_t id) override;
	virtual void*	 request(uint64_t id) override;
	virtual bool	 jumpToItem(uint64_t id) override;
};