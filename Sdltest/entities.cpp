#include "mainHead.hpp"
#include "entities.hpp"


entity_interface::entity_interface(float x, float y, float w, float h,float health):health(health)
{
	data.x = x; data.y = y; data.h = h; data.w = w;
	data_to_data();
}


SDL_FRect entity_interface::get_data() const noexcept
{
	return data;
}


SDL_Texture* entity_interface::get_texture()  noexcept
{
	return texture;
}


void entity_interface::set_data(SDL_FRect rhs) noexcept
{
	data = rhs;
}


void entity_interface::set_texture(SDL_Texture& rhs) noexcept
{
	texture = &rhs;
}


void entity_interface::change_coordinate(float x, float y) noexcept
{
	data.x += x;
	data.y += y;
}


void entity_interface::set_coordinate(float x, float y) noexcept
{
	data.x = x;
	data.y = y;
}


float entity_interface::get_health() const noexcept
{
	return health;
}


void entity_interface::set_health(float value) noexcept
{
	health = value;
}


void entity_interface::data_to_data() noexcept
{
	int_data.x = data.x;
	int_data.y = data.y;
}


entity_interface::~entity_interface()
{
}





Weapons::Weapons(float x, float y, float w, float h,float speed,float damage,double delay)
	:entity_interface(x, y, w, h, health),speed(speed),damage(damage),life(true),delay(delay) {}


bool Weapons::check_life() const noexcept
{
	return life;
}

void Weapons::change_life(bool value) noexcept
{
	life = value;
}

float Weapons::get_delay() const noexcept
{
	return delay; 
}

Weapons::~Weapons() {}



default_weapon::default_weapon(float x,float y) : Weapons (x, y, 30, 30,-10,10,0)
{
	texture = &texturs::load_main_bullet();
}


void default_weapon::hit() noexcept
{
	change_coordinate(0, speed);
}





mainHero::mainHero(float x, float y, float w, float h) : entity_interface( x, y, w, h,100)
{

	texture = &texturs::load_main_texture();
	stockman::push(default_weapon());
	entities_pointers::it <default_weapon> = stockman::get_head<default_weapon>();
	Weap = &*entities_pointers::it<default_weapon>;
	Weap->set_coordinate(data.x, data.y);
	delay = Weap->get_delay();
}


void mainHero::set_weapon(Weapons* weap) noexcept
{
	Weap = weap;
}


Weapons& mainHero::get_weapon()  noexcept
{
	return *Weap;
}

void mainHero::hit() noexcept 
{
	//checks if projectile has reached the screen bolder and checks the delay between shoots
	if (StopWatch::turn(delay) == true)
	{
		Weap = fun_logic
		(
			stockman::get_head<default_weapon>(),
			stockman::get_tail<default_weapon>(),
			&Weapons::check_life,
			true
		);


		if (Weap == nullptr)
		{
			stockman::push(default_weapon(data.x, data.y));
		}
		else
		{
			Weap->set_coordinate(data.x, data.y);
		}

	}
}



template<typename It,typename arg,typename T>
T* fun_logic(It beg,It end, arg T::*fun,bool flag)
{
	static_assert(std::is_same_v<decltype(beg),decltype(end)>,"The iterators should be equal");

	for (; beg != end; beg++)
	{
		auto it = *beg;
		bool temp=(it.*fun)();
		if (temp == flag)
			return &*beg;
	}
	return nullptr;
}





template<typename key>
stockman::it_contaiter_type<key> stockman::get_head() noexcept
{
	auto f = map<key>.find(std::type_index(typeid(decltype(std::declval<key>()))));
	if (f != map<key>.end())
	{
		return f->second.begin();
	}
}

template<typename key>
stockman::it_contaiter_type<key> stockman::get_tail() noexcept
{
	auto f = map<key>.find(std::type_index(typeid(decltype(std::declval<key>()))));
	if (f != map<key>.end())
	{
		return f->second.end();
	}
}


	template<typename T, typename U>
	 void stockman::push(T t) 
	{
		auto f = map<T>.find(std::type_index(typeid(t)));
		if (f != map<T>.end())
		{
			f->second.emplace_back(t);
		}
		else
		{
			map<T>[std::type_index(typeid(t))] = {t};
		}
	}



	template<typename key,typename U>
	 key& stockman::find() noexcept
	{
		auto f = map<key>.find(std::type_index(typeid(decltype(std::declval<key>()))));
		if (f != map<key>.end())
		{
			auto it = f->second.begin();
			return *it;
		}
	}

	 
	 template<typename It,typename U>
	 void stockman::move_to_begin(It it) noexcept
	 {
		 using type = std::remove_reference_t<decltype(*it)>;
		 auto f = map<type>.find(std::type_index(typeid(type)));
		 if (f != map.end())
		 {
			 f->second.splice(f->second.before_begin(), f->second, it);
		 }
		
	 }




	 template<typename It>
		 requires (std::_Is_iterator_v<It>)
	  void stockman::erase(It it) noexcept
	 {
		 using type = std::remove_reference_t<decltype(*it)>;
		 auto f = map<type>.find(std::type_index(typeid(type)));
		 if (f != map<type>.end())
		 {
			 f->second.erase(it);
		 }
	 }


	 template<typename T>
		 requires (std::is_pointer_v<T>)
	 bool entities_pointers::check_for_nullptr(T t)
	 {
		 if (t == nullptr)
			 return true;
		 else
			 return false;
	 }


	 template<typename It>
		 requires(std::_Is_iterator_v<It>)
	 bool entities_pointers::check_for_nullptr(It it)
	 {
		 using type = std::remove_reference_t<decltype(*it)>;

		 if (it == stockman::get_tail<type>())
			 return true;
		 else
			 return false;
	 }

