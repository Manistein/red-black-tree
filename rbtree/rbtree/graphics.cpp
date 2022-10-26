#include "graphics.h"
#include <corecrt_math.h>
#include <algorithm>
#include <time.h>

static void draw_control_panel(RBTree* tree) {
	static int index = 0;
	static time_t t = time(NULL);

	char buf[128] = { 0 };
	sprintf(buf, "%lld.txt", t);
	FILE* f = fopen(buf, "a+");

	// Control Pannel
	ImGui::Begin("Control Pannel");

	// insert
	static int v = 0;
	int temp = 0;
	if (ImGui::InputInt("", &temp, 0, 0)) {
		v = temp;
	}

	if (ImGui::Button("insert")) {
		index++;
		fprintf(f, "index:%d op:insert %d\n", index, v);
		rbtree_insert(tree, v);
	}

	if (ImGui::Button("delete")) {
		index++;
		fprintf(f, "index:%d op:delete %d\n", index, v);
		rbtree_remove(tree, v);
	}

	ImGui::End();

	fclose(f);
}

const float min_x = 60;
const float min_y = 60;
const float y_step = 60;
const float radius = 20;
const float font_shift = 8;

const ImU32 color[RBTN_COLOR_INVALID] = {
	IM_COL32(0,0,0,255),
	IM_COL32(255,0,0,255),
};

static void draw(RBTreeNode* node, float parent_x, float parent_y, float x_delta) {
	if (node == NULL)
		return;

	float x = 0.0f;
	float y = 0.0f;

	if (node->parent == NULL) {
		x = parent_x;
		y = parent_y;
	}
	else {
		if (node == node->parent->left) {
			x = parent_x - x_delta;
		}
		else {
			x = parent_x + x_delta;
		}

		y = parent_y + y_step;
	}

	if (node->left) {
		float cx = x - x_delta / 2;
		float cy = y + y_step;

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(cx, cy), IM_COL32(255, 255, 255, 255));
		draw(node->left, x, y, x_delta / 2);
	}

	if (node->right) {
		float cx = x + x_delta / 2;
		float cy = y + y_step;

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(cx, cy), IM_COL32(255, 255, 255, 255));
		draw(node->right, x, y, x_delta / 2);
	}

	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, color[node->color]);

	char buf[64];
	sprintf(buf, "%d", node->val);
	int len = strlen(buf);
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - (len / 2) * font_shift - font_shift / 2, y - font_shift), IM_COL32(255, 255, 255, 255), buf);
}

static void draw_tree(RBTree* rbtree) {
	static ImVec2 mouse_drag;

	if (ImGui::IsMouseDragging(0)) {
		ImVec2 drag_delta = ImGui::GetIO().MouseDelta;
		mouse_drag.x += drag_delta.x;
		mouse_drag.y += drag_delta.y;
	}

	int depth = rbtree_traverse(rbtree->root, 0, 1, NULL);
	int max_level_nodes = (int)pow(2, (double)(depth - 1));
	float width = (max_level_nodes * 2 * radius) + (max_level_nodes - 1) * radius;
	draw(rbtree->root, width / 2 + min_x + mouse_drag.x, min_y + mouse_drag.y, width / 2);
}

void draw_loop(RBTree* rbtree) {
	draw_control_panel(rbtree);
	draw_tree(rbtree);
}