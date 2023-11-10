#pragma once
#include "mainHead.hpp"


class entity_interface
{
protected:

	SDL_Rect  int_data{};
	SDL_FRect data{};
	SDL_Texture* texture{};

	float health{};

public:

	entity_interface(float x, float y, float w, float h,float health);

	virtual void data_to_data() noexcept;

	virtual void change_coordinate(float x, float y) noexcept;

	virtual void set_coordinate(float x, float y) noexcept;

	virtual SDL_FRect get_data() const noexcept;

	virtual SDL_Texture* get_texture()  noexcept;

	virtual void set_data(SDL_FRect rhs) noexcept;

	virtual void set_texture(SDL_Texture& rhs) noexcept;

	virtual float get_health() const  noexcept;

	virtual void set_health(float value) noexcept;

	virtual ~entity_interface()=0;
};

class Weapons :public entity_interface
{
protected:
	float damage, speed;
	double delay;
	bool life;

public:

	Weapons(float x, float y, float w, float h,float speed,float damage,double delay);

	virtual void hit() = 0;

	bool check_life() const noexcept;

	void change_life(bool value) noexcept;

	float get_delay() const noexcept;


	virtual ~Weapons();	
};


class default_weapon :public Weapons
{
public:

	default_weapon(float x=0, float y=0);

	void hit() noexcept override ;
};


class mainHero:public entity_interface 
{
protected:

	double delay;
	Weapons* Weap;

public:

	mainHero(float x, float y, float w, float h);

	void set_weapon(Weapons* weap) noexcept;

	Weapons& get_weapon() noexcept;

	void hit() noexcept;
};


//this function was created to return an entity whose function returned a value equal to the value of the flag
template<typename It,typename arg,typename T>
static T* fun_logic(It beg, It end, arg T::*fun,bool flag=false);



//this class shoud to store only all the texture(entity) 
class stockman
{

public:

	template<typename T>
	using container_type = std::list<T>;

	template<typename T>
	using it_contaiter_type = container_type<T>::iterator;

public:

	template<typename key>
	static it_contaiter_type<key> get_head() noexcept;

	template<typename key>
	static it_contaiter_type<key> get_tail() noexcept;

public:

	template<typename T,
		typename U = std::enable_if_t<std::is_base_of_v<entity_interface, T>>>
	static void push(T t);

	template<typename key,
		typename U = std::enable_if_t<std::is_base_of_v<entity_interface, key>>>
	static  key& find() noexcept;


	template<typename It,
		typename U = std::enable_if_t<std::_Is_iterator_v<It>>>
	static  void move_to_begin(It it) noexcept;


	template<typename It>
		requires (std::_Is_iterator_v<It>)
	inline void erase(It it) noexcept;


private:
	template<typename T>
	static inline std::unordered_map<std::type_index, container_type<T>> map;

};


//pointers into list of entities
struct entities_pointers
{
	static inline Weapons* weaponer{};

	template<typename T>
	using Iterator = typename stockman::it_contaiter_type<T>;

	template<typename T>
	static inline Iterator<T> it{};

	template<typename T>
		requires (std::is_pointer_v<T>)
	static inline bool check_for_nullptr(T t);

	template<typename It>
		requires(std::_Is_iterator_v<It>)
	static inline bool check_for_nullptr(It it);

};



template<typename It>
	requires (std::_Is_iterator_v<It>)
static inline void destroy_obj(It it)
{
	stockman::erase(it);
}


