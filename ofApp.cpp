#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);

	for (int i = 0; i < 49; i++) {

		this->param_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	auto len = 25;
	ofColor color;

	for (int k = 0; k < this->param_list.size(); k++) {

		auto head_size = 8;
		ofSetColor(39);
		for (int j = 0; j < 2; j++) {

			ofPushMatrix();
			ofTranslate((k % 7) * 100 - 300, (k / 7) * 100 - 300);

			ofMesh mesh;
			vector<glm::vec3> right, left, frame;

			glm::vec3 last_location;
			float last_theta, last_radius;

			for (int i = 0; i < len; i++) {

				auto noise_deg = ofMap(ofNoise(this->param_list[k].x, (ofGetFrameNum() + i) * 0.01), 0, 1, -360, 360);
				auto noise_radius = ofMap(ofNoise(this->param_list[k].y, (ofGetFrameNum() + i) * 0.015), 0, 1, -50, 50);
				auto next_noise_deg = ofMap(ofNoise(this->param_list[k].x, (ofGetFrameNum() + i + 1) * 0.01), 0, 1, -360, 360);
				auto next_noise_radius = ofMap(ofNoise(this->param_list[k].y, (ofGetFrameNum() + i + 1) * 0.015), 0, 1, -50, 50);

				auto location = glm::vec3(noise_radius * cos(noise_deg * DEG_TO_RAD), noise_radius * sin(noise_deg * DEG_TO_RAD), 0);
				auto next = glm::vec3(next_noise_radius * cos(next_noise_deg * DEG_TO_RAD), next_noise_radius * sin(next_noise_deg * DEG_TO_RAD), 0);

				auto direction = next - location;
				auto theta = atan2(direction.y, direction.x);

				right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta + PI * 0.5), 0));
				left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta - PI * 0.5), 0));

				last_location = location;
				last_theta = theta;
				last_radius = noise_radius;
			}

			for (int i = 0; i < right.size(); i++) {

				mesh.addVertex(left[i]);
				mesh.addVertex(right[i]);
			}

			for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

				mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
				mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
			}

			auto tmp_head_size = ofMap(mesh.getNumVertices() - 2, 0, mesh.getNumVertices(), 0, head_size);

			mesh.addVertex(last_location);
			int index = mesh.getNumVertices();
			for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

				mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
				frame.push_back(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
			}

			for (int i = index; i < mesh.getNumVertices() - 1; i++) {

				mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
			}

			if (abs(last_radius) < 2) { this->param_list[k].z += 0.1; }

			int span = ofMap(ofNoise(this->param_list[k].z), 0, 1, 1, 7);
			int deg_span = 360 / span;
			for (int deg = 0; deg < 360; deg += 30) {

				ofRotate(deg_span);
				mesh.draw();
			}

			ofPopMatrix();

			head_size -= 3;

			color.setHsb(ofMap((k % 7) * 100 - 300, -300, 300, 0, 255), 180, 255);
			ofSetColor(color);
		}
	}

	int start = 75;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
