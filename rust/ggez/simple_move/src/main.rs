// playing around with the super simple example from the ggez examples

extern crate ggez;

use ggez::event;
use ggez::event::{KeyCode, KeyMods};
use ggez::graphics;
use ggez::nalgebra as na;
use ggez::{Context, GameResult};

struct MainState {
    dir : Direction,
    pos_x: f32,
    pos_y: f32,
    speed: f32,
}

impl MainState {
    fn new(_ctx: &mut Context) -> GameResult<MainState> {
        let s = MainState { dir: Direction::Right , pos_x: 400.0, pos_y: 400.0 , speed: 5.0 };
        Ok(s)
    }

}

impl event::EventHandler for MainState {
    fn update(&mut self, _ctx: &mut Context) -> GameResult {
        self.pos_x = self.pos_x % 800.0;
        self.pos_y = self.pos_y % 800.0;
        Ok(())
    }

    fn draw(&mut self, ctx: &mut Context) -> GameResult {
        graphics::clear(ctx, [0.1, 0.2, 0.3, 1.0].into());

        let circle = graphics::Mesh::new_circle(
            ctx,
            graphics::DrawMode::fill(),
            na::Point2::new(self.pos_x, self.pos_y),
            100.0,
            2.0,
            graphics::WHITE,
        )?;
        graphics::draw(ctx, &circle, (na::Point2::new(0.0, 0.0),))?;

        graphics::present(ctx)?;
        Ok(())
    }

    fn key_down_event(
        &mut self,
        _ctx: &mut Context,
        keycode: KeyCode,
        _keymod: KeyMods,
        _repeat: bool,
    ) {
        self.dir = Direction::from_keycode(keycode).expect("KeyDown event failed\n");

        match self.dir {
            Direction::Up => self.pos_y = self.pos_y - self.speed,
            Direction::Down => self.pos_y = self.pos_y + self.speed,
            Direction::Left => self.pos_x = self.pos_x - self.speed,
            Direction::Right => self.pos_x = self.pos_x + self.speed,
        };
    }
}

/// for direction:
enum Direction {
    Up,
    Down,
    Left,
    Right,
}

impl Direction {
    /// get a direction from keycode
    pub fn from_keycode(key: KeyCode) -> Option<Direction> {
        match key {
            KeyCode::Up => Some(Direction::Up),
            KeyCode::Down => Some(Direction::Down),
            KeyCode::Left => Some(Direction::Left),
            KeyCode::Right => Some(Direction::Right),
            _ => None,
        }
    }
}



pub fn main() -> GameResult {
    let cb = ggez::ContextBuilder::new("super_simple", "ggez");
    let (ctx, event_loop) = &mut cb.build()?;
    let state = &mut MainState::new(ctx)?;
    event::run(ctx, event_loop, state)
}
