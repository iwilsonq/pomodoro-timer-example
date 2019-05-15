type state = {
  seconds: int,
  isTicking: bool,
};

type action =
  | Start
  | Stop
  | Reset
  | Tick;

let padNumber = numString =>
  if (numString |> int_of_string < 10) {
    "0" ++ numString;
  } else {
    numString;
  };

let formatTime = seconds => {
  let mins = seconds / 60;
  let minsString = mins |> string_of_int |> padNumber;
  let seconds = seconds mod 60;
  let secondsString = seconds |> string_of_int |> padNumber;
  minsString ++ ":" ++ secondsString;
};

module Button = {
  [@react.component]
  let make = (~label, ~onClick) => {
    <button onClick> {label |> ReasonReact.string} </button>;
  };
};

let updateTitle: string => unit = [%bs.raw
  {|
  function updateTitle(remaining) {
    document.title = "⏰ " + remaining + " ⏰";
  }|}
];

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Start => {...state, isTicking: true}
        | Stop => {...state, isTicking: false}
        | Reset => {...state, seconds: 30}
        | Tick =>
          state.isTicking && state.seconds > 0
            ? {
              updateTitle(formatTime(state.seconds - 1));
              {...state, seconds: state.seconds - 1};
            }
            : state
        },
      {isTicking: false, seconds: 30},
    );

  React.useEffect0(() => {
    let timerId = Js.Global.setInterval(() => dispatch(Tick), 1000);
    Some(() => Js.Global.clearInterval(timerId));
  });

  <div
    style={ReactDOMRe.Style.make(
      ~border="1px solid black",
      ~borderRadius="8px",
      ~maxWidth="180px",
      ~textAlign="center",
      (),
    )}>
    <p
      style={ReactDOMRe.Style.make(
        ~color="#444444",
        ~fontSize="42px",
        ~margin="16px 0",
        (),
      )}>
      {state.seconds |> formatTime |> ReasonReact.string}
    </p>
    {state.isTicking
       ? <Button label="STOP" onClick={_event => dispatch(Stop)} />
       : <>
           <Button label="START" onClick={_event => dispatch(Start)} />
           <Button label="RESET" onClick={_event => dispatch(Reset)} />
         </>}
  </div>;
};