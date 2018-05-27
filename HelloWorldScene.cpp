#include "HelloWorldScene.h"
#include "AppMacros.h"
#include <vector>
#include <string>

#include "../../external//xxtea/xxtea.h"

using namespace std;
USING_NS_CC;


Scene* HelloWorld::scene()
{
     return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/arial.ttf", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite);

	ProcessLua();
	//ProcessJpg();
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::ProcessLua() {
	string path = "xxx(apk_path)/assets/src_new/";
	string sign = "sign";
	int sign_len = 0;
	char* key = "key";
	int key_len = 0;

	vector<string>* files = new vector<string>();
	FileUtils::getInstance()->listFilesRecursively(path, files);

	for (int i = 0; i < (*files).size(); ++i)
	{
		string filePath = (*files)[i];
		char end = filePath[filePath.size() - 1];
		string ext = FileUtils::getInstance()->getFileExtension(filePath);
		if ( ext.size() > 0 && ext == ".luac") {
			Data DATA = FileUtils::getInstance()->getDataFromFile(filePath);
			if (!DATA.isNull()) {
				bool isXXTEA = DATA.getSize() > sign_len;
				xxtea_long len = 0;
				if (isXXTEA) {
					unsigned char *buffer = xxtea_decrypt(DATA.getBytes() + sign_len,
						(xxtea_long)DATA.getSize() - sign_len,
						(unsigned char *)key,
						(xxtea_long)key_len,
						&len);

					if (buffer != NULL) {
						FileUtils::getInstance()->removeFile(filePath);

						DATA.copy(buffer, len);

						string newPath = filePath.substr(0, filePath.size() - 1);
						FileUtils::getInstance()->writeDataToFile(DATA, newPath);
					}
					else {
						CCLOG("file error2 is: %s", filePath.c_str());
					}

				}
				else {

				}
			}
			else {
				CCLOG("file error is: %s", filePath.c_str());
			}
		}

	}

}

void HelloWorld::ProcessJpg() {
	string path = "xxx/assets/res_new/";
	string sign = "sign";
	int sign_len = 0;
	char* key = "key";
	int key_len = 0;

	vector<string>* files = new vector<string>();
	FileUtils::getInstance()->listFilesRecursively(path, files);

	for (int i = 0; i < (*files).size(); ++i)
	{
		string filePath = (*files)[i];
		char end = filePath[filePath.size() - 1];
		string ext = FileUtils::getInstance()->getFileExtension(filePath);
		if (ext.size() > 0 && (ext == ".jpg" || ext == ".png")) {
			Data DATA = FileUtils::getInstance()->getDataFromFile(filePath);
			if (!DATA.isNull()) {
				bool isXXTEA = DATA.getSize() > 0;
				xxtea_long len = 0;
				if (isXXTEA) {
					unsigned char *buffer = xxtea_decrypt(DATA.getBytes(),
						(xxtea_long)DATA.getSize(),
						(unsigned char *)sign.c_str(),
						(xxtea_long)sign_len,
						&len);

					if (buffer != NULL) {
						FileUtils::getInstance()->removeFile(filePath);

						DATA.copy(buffer, len);
						FileUtils::getInstance()->writeDataToFile(DATA, filePath);
					}
					else {
						CCLOG("file error2 is: %s", filePath.c_str());
					}

				}
				else {

				}
			}
			else {
				CCLOG("file error is: %s", filePath.c_str());
			}
		}

	}
}
